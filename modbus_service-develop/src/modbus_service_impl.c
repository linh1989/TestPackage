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

#define MAX_SOCKET_FD_INVALID -1

#define MS_TO_NS_CONVERT 1000000

#define MODBUS_TX_TIMEBASE_EXP_FLAG 1
#define MODBUS_MAX_RECEIVERS 20
#define MODBUS_RX_BUFF_SIZE 32

#define MODBUS_NODE_COUNT 2

#define MODBUS_EMETER_OFFSET      0
#define MODBUS_ISOCHA_OFFSET      1
#define MODBUS_ALL_NODE_COUNT     2

#define COMM_EMETER_LOST_MS        15000
#define COMM_ISOCHA_LOST_MS        15000

#define REQUEST_MESSAGE_PERIOD     3000

#define COMM_EMETER_LOST_TICK  (COMM_EMETER_LOST_MS / REQUEST_MESSAGE_PERIOD)
#define COMM_ISOCHA_LOST_TICK  (COMM_ISOCHA_LOST_MS / REQUEST_MESSAGE_PERIOD)

typedef struct
{
    uint8_t timeCheck;
    modbus_connection_status_t connect_status;
    pthread_mutex_t m_connection;
} modbus_nodestatus_t;

typedef struct {
    uint8_t          counter;
    uint8_t          max;
    modbus_nodes_t  node;
    modbus_module_t module;
    bool             status;
} modbus_node_monitor_t;

//==================================================================

// Modbus handlers
static modbus_service_err_t modbus_run(void);
static modbus_service_err_t register_event(modbus_event_t event);
static modbus_service_err_t get_emeter_output(modbus_emeter_output_t* emeter_output);
static modbus_service_err_t get_emeter_version_info(modbus_version_t* version);
static modbus_service_err_t get_isocha_version_info(modbus_version_t* version);
static modbus_service_err_t get_emeter_status_info(uint32_t* error_code);
static modbus_service_err_t get_isocha_status_info(uint32_t* error_code);
static modbus_service_err_t set_system_reboot(void);
static modbus_service_err_t set_isocha_measurement_state(bool start);

// Modbus subjects
static int register_receiver(modbus_impl_receiver *receiver);
static int unregister_receiver(modbus_impl_receiver *receiver);
static void notify_receiver(modbus_response_t response, void *data);

// Threads
static void *modbus_tx_thread(void *context);
static void *modbus_rx_thread(void *context);
static void modbus_tx_timer_handler(int signo);

static void modbus_rx_polling_group_event_changes(void);

//==================================================================
static pthread_t modbus_tx_tid;
static pthread_t modbus_rx_tid;
static pthread_mutex_t modbus_tx_mutex;
static pthread_cond_t modbus_tx_cond_var;
static uint32_t tx_events_flag = 0;
static pthread_mutex_t receiver_mutex;
static uint32_t receiver_map = 0;
static modbus_impl_receiver *receivers[MODBUS_MAX_RECEIVERS];

static bool iso_start = false;

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

static bool comm_req[MODBUS_ALL_NODE_COUNT] = {
    false, // MODBUS_NODE_EMETER
    false, // MODBUS_NODE_ISOCHA
};

static modbus_node_monitor_t node_monitor[MODBUS_ALL_NODE_COUNT] = {
    { .counter = 0, .max = COMM_EMETER_LOST_TICK, .node = MODBUS_NODE_EMETER,  .module = EMETER_MODULE,   .status = true },
    { .counter = 0, .max = COMM_ISOCHA_LOST_TICK, .node = MODBUS_NODE_ISOCHA , .module = ISOCHA_MODULE,   .status = true },
};

static modbus_service_err_t modbus_run(void)
{
    modbus_module_t i;
    memset(receivers, 0, sizeof(receivers));

    modbus_rtu_init();

    for (i = 0; i < NUMBER_OF_MODBUS_MODULE; i++)
    {
        nodeStatus[i].timeCheck = 0;
        nodeStatus[i].connect_status = CAN_CONNECTED;
        pthread_mutex_init(&nodeStatus[i].m_connection, NULL);
    }

    /* Start CanTP Handle */
    modbus_rtu_StartService();    
    
    pthread_attr_t tattr;    
    size_t size = 0x20000;//128K
    /* initialized with default attributes */
    pthread_attr_init(&tattr);
    /* setting the size of the stack also */
    pthread_attr_setstacksize(&tattr, size);    
    pthread_create(&modbus_tx_tid, &tattr, modbus_tx_thread, NULL);        

    /* initialized with default attributes */
    pthread_attr_init(&tattr);
    /* setting the size of the stack also */
    pthread_attr_setstacksize(&tattr, size);    
    pthread_create(&modbus_rx_tid, &tattr, modbus_rx_thread, NULL);    

    return MODBUS_SERVICE_ERR_NONE;
}

bool modbus_get_connect_status(modbus_module_t i)
{
    switch (i) {
        case EMETER_MODULE:
            return !node_monitor[MODBUS_EMETER_OFFSET].status;

        case ISOCHA_MODULE:
            return !node_monitor[MODBUS_ISOCHA_OFFSET].status;

        default:
            return false;
    }
}

static modbus_service_err_t modbus_set_connected(modbus_msg_id_t msg_id)
{
    modbus_module_t i = NUMBER_OF_MODBUS_MODULE;
    uint8_t j;

    for (i = 0; i < NUMBER_OF_MODBUS_MODULE; i++)
    {
        if ((i == EMETER_MODULE) || (i == ISOCHA_MODULE))
        {
            // Always connected for this release
            pthread_mutex_lock(&nodeStatus[i].m_connection);
            nodeStatus[i].timeCheck = 0;
            nodeStatus[i].connect_status = MODBUS_CONNECTED;
            pthread_mutex_unlock(&nodeStatus[i].m_connection);
            return MODBUS_SERVICE_ERR_NONE;
        }
    }

    return MODBUS_SERVICE_ERR_NOT_CONNECTED;
}

static modbus_service_err_t register_event(modbus_event_t event)
{ 
    switch (event) { 
        default:
            break;
    }
    
    return MODBUS_SERVICE_ERR_NONE;
}

static modbus_service_err_t get_emeter_output(modbus_emeter_output_t *emeter_output)
{
    SLOGI("%s", "Get emeter output info");

    assert(emeter_output);

    return MODBUS_SERVICE_ERR_NONE;
}

static modbus_service_err_t get_emeter_version_info(modbus_version_t* version)
{
    SLOGI("%s", "Get emeter version info");

    assert(version);

    return MODBUS_SERVICE_ERR_NONE;
}

static modbus_service_err_t get_isocha_version_info(modbus_version_t* version)
{
    SLOGI("%s", "Get isocha version info");

    assert(version);

    return MODBUS_SERVICE_ERR_NONE;
}

static modbus_service_err_t get_emeter_status_info(uint32_t* error_code)
{
    SLOGI("%s", "Get emeter error_code info");

    assert(error_code);

    return MODBUS_SERVICE_ERR_NONE;
}

static modbus_service_err_t get_isocha_status_info(uint32_t* error_code)
{
    SLOGI("%s", "Get isocha error_code info");

    assert(error_code);

    return MODBUS_SERVICE_ERR_NONE;
}

static modbus_service_err_t set_system_reboot(void)
{
    FUNC_ENTRY();
    SLOGI("%s", "Set system reboot");

    FUNC_EXIT();
    return MODBUS_SERVICE_ERR_NONE;
}

static modbus_service_err_t set_isocha_measurement_state(bool start)
{
    FUNC_ENTRY();
    SLOGI("Set Isolation Measurement State: %d", start);

    FUNC_EXIT();
    return MODBUS_SERVICE_ERR_NONE;
}

void *modbus_tx_thread(void *context)
{
    FUNC_ENTRY();
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_handler = modbus_tx_timer_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGRTMIN, &sa, NULL);

    struct sigevent sev;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;

    timer_t timer_id;
    timer_create(CLOCK_MONOTONIC, &sev, &timer_id);

    // Start periodic timer (100ms)
    struct itimerspec its;
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = CAN_MESSAGE_PERIOD * MS_TO_NS_CONVERT;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = CAN_MESSAGE_PERIOD * MS_TO_NS_CONVERT;
    timer_settime(timer_id, 0, &its, NULL);

    pthread_mutex_lock(&modbus_tx_mutex);
    while (true)
    {
        if (tx_events_flag)
        {
            uint32_t cp_events_flag = tx_events_flag;
            pthread_mutex_unlock(&modbus_tx_mutex);

            if (cp_events_flag & MODBUS_TX_TIMEBASE_EXP_FLAG)
            {
                for (uint8_t i = 0; i < MODBUS_RTU_MSG_MAX; i++)
                {
                    rtu_msg_t *rtu_msg = modbus_rtu_get_msg(i);
                    if ((rtu_msg->type == MODBUS_TX_MSG) && rtu_msg->active)
                    {
                        rtu_msg->elapsed += RTU_MESSAGE_PERIOD;
                        if (rtu_msg->elapsed >= rtu_msg->period_ms)
                        {
                            rtu_msg->elapsed = 0;

                            modbus_service_err_t err = modbus_rtu_send(i);
                            if (err != MODBUS_SERVICE_ERR_NONE)
                            {
                                SLOGI("Msg id %d periodic sent failed", i);
                            }

                            if (bus_idle_count >= BUS_IDLE_ASSERT)
                            {
                                SLOGI("%s", "Exit bus idle state: Tx");
                                bus_idle_count = 0;
                                bool is_idle = false;
                            }
                        }
                    }
                }

                tx_events_flag ^= MODBUS_TX_TIMEBASE_EXP_FLAG;
            }
            pthread_mutex_lock(&modbus_tx_mutex);
        }
        else
        {
            pthread_cond_wait(&modbus_tx_cond_var, &modbus_tx_mutex);
        }

        if (true)
        {
            static uint32_t cnt = 0;
            if (cnt >= 5)
            {
                cnt = 0;
                //Polling each 500ms
                //modbus_rx_polling_group_event_changes();
            }
            else
            {
                cnt++;
            }
        }
    }

    FUNC_EXIT();
    return NULL;
}

void *modbus_rx_thread(void *context)
{
    FUNC_ENTRY();

    while (true)
    {
        // Handle RX Messages

    }

    FUNC_EXIT();
    return NULL;
}

static void modbus_tx_timer_handler(int signo)
{
    // Bus idle check
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
