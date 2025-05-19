/****************************************************
 *  modbus_service_impl.c
 *  Created on: 03-May-2025 21:03:54
 *  Implementation of the Class modbus_service_impl
 *  Original author: LinhNM
 ****************************************************/

#define LOG_TAG "modbus_service_impl"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>

#include "logger.h"
#include "modbus_service_impl.h"
#include "modbus_service_utils.h"
//==================================================================

#define MS_TO_NS_CONVERT 1000000

#define MODBUS_TX_TIMEBASE_EXP_FLAG 1
#define MODBUS_MAX_RECEIVERS 20
#define MODBUS_RX_BUFF_SIZE 32

#define MODBUS_NODE_COUNT 2

#define COMM_EMETER_LOST_MS        15000
#define COMM_ISOCHA_LOST_MS        15000

#define REQUEST_MESSAGE_PERIOD     1000

#define MAX_NB_REGS                10

#define EMETER_BASE_REG_ADDRESS    0x00
#define ISOCHA_BASE_REG_ADDRESS    0x00

#define COMM_EMETER_LOST_TICK  (COMM_EMETER_LOST_MS / REQUEST_MESSAGE_PERIOD)
#define COMM_ISOCHA_LOST_TICK  (COMM_ISOCHA_LOST_MS / REQUEST_MESSAGE_PERIOD)

//==================================================================

// Modbus handlers
static modbus_err_t modbus_run(void);
static modbus_err_t register_event(modbus_event_t event);
static modbus_err_t get_emeter_output(modbus_emeter_output_t* emeter_output);
static modbus_err_t get_emeter_version_info(modbus_version_t* version);
static modbus_err_t get_isocha_version_info(modbus_version_t* version);
static modbus_err_t get_emeter_status_info(uint32_t* error_code);
static modbus_err_t get_isocha_status_info(uint32_t* error_code);
static modbus_err_t set_system_reboot(void);
static modbus_err_t set_isocha_measurement_state(bool start);

// Modbus subjects
static int register_receiver(modbus_impl_receiver *receiver);
static int unregister_receiver(modbus_impl_receiver *receiver);
static void notify_receiver(modbus_response_t response, void *data);

// Threads
static void *modbus_rtu_thread(void *context);
static void modbus_tx_timer_handler(int signo);

//==================================================================
static pthread_t modbus_rtu_tid;
static pthread_mutex_t receiver_mutex;
static uint32_t receiver_map = 0;
static modbus_impl_receiver *receivers[MODBUS_MAX_RECEIVERS];

static bool iso_start = false;
modbus_version_t tmp_emeter_output;
modbus_version_t tmp_isocha_version;
uint32_t tmp_emeter_error_code = 0;
uint32_t tmp_isocha_error_code = 0;
modbus_iso_status_t tmp_isocha_iso_status = ISOCHA_ISO_INVALID;
static uint32_t cnt = 0;
static uint32_t cnt2 = 990;

static modbus_impl_subject modbus_subject = {
    .register_receiver = register_receiver,
    .unregister_receiver = unregister_receiver,
    .notify = notify_receiver,
};

static modbus_impl_handle_t modbus_handle = {
    .run = modbus_run,
    .register_event = register_event,
    .get_emeter_output = get_emeter_output,
    .get_emeter_version_info = get_emeter_version_info,
    .get_isocha_version_info = get_isocha_version_info,
    .get_emeter_status_info = get_emeter_status_info,
    .get_isocha_status_info = get_isocha_status_info,
    .set_system_reboot = set_system_reboot,
    .set_isocha_measurement_state = set_isocha_measurement_state,
};

static modbus_err_t modbus_run(void)
{
    modbus_module_t i;
    memset(receivers, 0, sizeof(receivers));

init_modbus:
    if (modbus_service_intf_init() != MODBUS_ERR_NONE) {
        SLOGE("Unable to init modbus channel. Try again after 30s");
        sleep(30);
        goto init_modbus;
    }
    
    SLOGE("Modbus channel established completely. Start main function");

    pthread_attr_t tattr;    
    size_t size = 0x20000;//128K
    /* initialized with default attributes */
    pthread_attr_init(&tattr);
    /* setting the size of the stack also */
    pthread_attr_setstacksize(&tattr, size);    
    pthread_create(&modbus_rtu_tid, &tattr, modbus_rtu_thread, NULL);         

    return MODBUS_ERR_NONE;
}

void *modbus_rtu_thread(void *context)
{
    FUNC_ENTRY();
    // Create the timer and set signal handler
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_handler = modbus_tx_timer_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGRTMIN, &sa, NULL);

    struct sigevent sev;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    uint16_t read_reg[MAX_NB_REGS];

    timer_t timer_id;
    timer_create(CLOCK_MONOTONIC, &sev, &timer_id);

    // Start periodic timer (100ms)
    struct itimerspec its;
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = REQUEST_MESSAGE_PERIOD * MS_TO_NS_CONVERT;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = REQUEST_MESSAGE_PERIOD * MS_TO_NS_CONVERT;
    timer_settime(timer_id, 0, &its, NULL);

    while (true)
    {
        
    }

    FUNC_EXIT();
    return NULL;
}

static void modbus_tx_timer_handler(int signo)
{
    FUNC_ENTRY();
    // Read Emeter Error code
    if (modbus_intf_read_request(MODBUS_NODE_EMETER, EMETER_BASE_REG_ADDRESS, MAX_NB_REGS, read_reg) !=  MODBUS_ERR_NONE) {
        SLOGE("Unable to read meter error code. Try again after 1s");
    } else {
        if (tmp_emeter_error_code != (uint32_t) read_reg[0]) {
            tmp_emeter_error_code = (uint32_t) read_reg[0];
            modbus_subject.notify(MODBUS_RESPONSE_GET_EMETER_STATUS, &tmp_emeter_error_code);
        }
    }

    // Read IsoCHA Error code
    if (modbus_intf_read_request(MODBUS_NODE_ISOCHA, ISOCHA_BASE_REG_ADDRESS, MAX_NB_REGS, read_reg) !=  MODBUS_ERR_NONE) {
        SLOGE("Unable to read isolation error code. Try again after 1s");
    } else {
        if (tmp_isocha_error_code != (uint32_t) read_reg[0]) {
            tmp_isocha_error_code = (uint32_t) read_reg[0];
            modbus_subject.notify(MODBUS_RESPONSE_GET_ISOCHA_STATUS, &tmp_isocha_error_code);
        }
    }

    if (iso_start) {
        // Read Isolation Status
        if (modbus_intf_read_request(MODBUS_NODE_ISOCHA, ISOCHA_BASE_REG_ADDRESS, MAX_NB_REGS, read_reg) !=  MODBUS_ERR_NONE) {
            SLOGE("Unable to read isolation info. Try again after 1s");
        } else {
            if (tmp_isocha_iso_status != (modbus_iso_status_t) read_reg[0]) {
                tmp_isocha_iso_status = (modbus_iso_status_t) read_reg[0];
                modbus_subject.notify(MODBUS_RESPONSE_SET_ISO_MEASURE_STATE, &tmp_isocha_iso_status);
            }
        }
    }

    if (cnt >= 5)
    {
        cnt = 0;
            if (is_charging) {
            // Read emeter output status
            if (modbus_intf_read_request(MODBUS_NODE_EMETER, EMETER_BASE_REG_ADDRESS, MAX_NB_REGS, read_reg) !=  MODBUS_ERR_NONE) {
                SLOGE("Unable to read emeter output info. Try again after 5s");
            } else {
                tmp_emeter_output.m_id = 0;
                tmp_emeter_output.voltage = 0;
                tmp_emeter_output.current = 0;
                tmp_emeter_output.power = 0;
            }
        }
    }
    else
    {
        cnt++;
    }

    if (cnt2 >= 1000)
    {
        cnt2 = 0;
        // Read emeter version
        if (modbus_intf_read_request(MODBUS_NODE_EMETER, EMETER_BASE_REG_ADDRESS, MAX_NB_REGS, read_reg) !=  MODBUS_ERR_NONE) {
            SLOGE("Unable to read modbus emeter version info. Try again after 1000s");
        } else {
            tmp_emeter_version.major = 1;
            tmp_emeter_version.minor = 0;
            tmp_emeter_version.subminor = 0;
        }
        // Read isoCHA version
        if (modbus_intf_read_request(MODBUS_NODE_ISOCHA, ISOCHA_BASE_REG_ADDRESS, MAX_NB_REGS, read_reg) !=  MODBUS_ERR_NONE) {
            SLOGE("Unable to read isocha version info. Try again after 1000s");
        } else {
            tmp_isocha_version.major = 1;
            tmp_isocha_version.minor = 0;
            tmp_isocha_version.subminor = 0;
        }
    }
    else
    {
        cnt2++;
    }
    FUNC_ENTRY();
}

static modbus_err_t register_event(modbus_event_t event)
{ 
    switch (event) { 
        default:
            break;
    }
    
    return MODBUS_ERR_NONE;
}

static modbus_err_t get_emeter_output(modbus_emeter_output_t *emeter_output)
{
    SLOGI("%s", "Get emeter output info");

    assert(emeter_output);
    *emeter_output = tmp_emeter_output;

    return MODBUS_ERR_NONE;
}

static modbus_err_t get_emeter_version_info(modbus_version_t* version)
{
    SLOGI("%s", "Get emeter version info");

    assert(version);
    *version = tmp_emeter_version;

    return MODBUS_ERR_NONE;
}

static modbus_err_t get_isocha_version_info(modbus_version_t* version)
{
    SLOGI("%s", "Get isocha version info");

    assert(version);
    *version = tmp_isocha_version;

    return MODBUS_ERR_NONE;
}

static modbus_err_t get_emeter_status_info(uint32_t* error_code)
{
    SLOGI("%s", "Get emeter error_code info");

    assert(error_code);
    *error_code = tmp_emeter_error_code

    return MODBUS_ERR_NONE;
}

static modbus_err_t get_isocha_status_info(uint32_t* error_code)
{
    SLOGI("%s", "Get isocha error_code info");

    assert(error_code);
    *error_code = tmp_isocha_error_code

    return MODBUS_ERR_NONE;
}

static modbus_err_t set_system_reboot(void)
{
    FUNC_ENTRY();
    SLOGI("%s", "Set system reboot");

    FUNC_EXIT();
    return MODBUS_ERR_NONE;
}

static modbus_err_t set_isocha_measurement_state(bool start)
{
    FUNC_ENTRY();
    SLOGI("Set Isolation Measurement State: %d", start);
    iso_start = start;

    FUNC_EXIT();
    return MODBUS_ERR_NONE;
}

const modbus_impl_handle_t *modbus_impl_get_handle(void)
{
    return &modbus_handle;
}

modbus_impl_subject *modbus_impl_get_subject(void)
{
    return &modbus_subject;
}

static int register_receiver(modbus_impl_receiver *receiver)
{
    assert(receiver);

    int retval = EXIT_SUCCESS;

    pthread_mutex_lock(&receiver_mutex);
    uint8_t pos = (uint8_t)ffsl(~receiver_map);
    if (!pos || (pos > MODBUS_MAX_RECEIVERS))
    {
        // No more receiver available
        retval = EXIT_FAILURE;
    }
    else
    {
        pos--;
        receiver_map |= (1 << pos);
        receivers[pos] = receiver;
        SLOGI("Receiver %d registered, map 0x%X", pos, receiver_map);
    }
    pthread_mutex_unlock(&receiver_mutex);

    return retval;
}

static int unregister_receiver(modbus_impl_receiver *receiver)
{
    assert(receiver);

    int retval = EXIT_FAILURE;

    pthread_mutex_lock(&receiver_mutex);
    uint32_t map = receiver_map;
    while (map)
    {
        uint8_t pos = (uint8_t)ffsl(map);
        if (pos)
        {
            pos--;
            if (receivers[pos] == receiver)
            {
                receivers[pos] = NULL;
                receiver_map &= ~(1 << pos);
                SLOGI("Receiver %d unregistered, map 0x%X", pos, receiver_map);
                retval = EXIT_SUCCESS;
                break;
            }
            map &= ~(1 << pos);
        }
    }
    pthread_mutex_unlock(&receiver_mutex);

    return retval;
}

static void notify_receiver(modbus_response_t response, void *data)
{
    FUNC_ENTRY();

    modbus_event_t event = resp_event_tbl[response];
    if (event >= MODBUS_EVENT_ALL)
    {
        SLOGE("Invalid event %d from resp %d", event, response);
    }
    else
    {
        pthread_mutex_lock(&receiver_mutex);
        uint32_t map = receiver_map;
        while (map)
        {
            uint8_t pos = (uint8_t)ffsl(map);
            if (pos)
            {
                pos--;
                if (receivers[pos]->register_event[event])
                {
                    SLOGI("Receiver %d notified response %d", pos, response);
                    receivers[pos]->on_notify(response, data, receivers[pos]->ctx);
                }
                map &= ~(1 << pos);
            }
        }
        pthread_mutex_unlock(&receiver_mutex);
    }

    FUNC_EXIT();
}

modbus_impl_receiver *modbus_impl_create_receiver(void *context, notification_handler_t ntfy)
{
    FUNC_ENTRY();

    modbus_impl_receiver *receiver = (modbus_impl_receiver *)malloc(sizeof(modbus_impl_receiver));
    if (receiver)
    {
        receiver->ctx = context;
        receiver->on_notify = ntfy;
        memset(receiver->register_event, 0, sizeof(receiver->register_event));
    }
    else
    {
        SLOGE("%s", "Receiver create error");
    }

    FUNC_EXIT();
    return receiver;
}
