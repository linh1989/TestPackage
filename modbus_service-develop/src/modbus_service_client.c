/****************************************************
 *  modbus_service_client.c
 *  Created on: 15-May-2025 16:27:24
 *  Implementation of the Class modbus_service_client.c
 *  Original author: linhnm
 ****************************************************/

#define LOG_TAG "modbus_service_client"

#include "modbus_service_client.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#include "rpc.h"
#include "logger.h"
#include "protobuf-c/rpc.pb-c.h"

#include "protobuf-c/modbus_service.pb-c.h"
#include "modbus_service_rpc_adapter.h"
#include "modbus_service.h"
#include "modbus_service_types.h"
#include "modbus_service_if.h"

#define BUFFER_EVENT_SOCKET_FD -1

#define RECONNECT_WAIT_TIME 1
#define MSG_BUFF_LENTH_MAX 128

#define RESPONSE_CHECK_INTERVAL 100 // us
#define RESPONSE_CHECK_MAX 100

#define CHECK_CLIENT_STATUS()                   \
    do                                          \
    {                                           \
        if (!is_initialized)                    \
        {                                       \
            return MODBUS_ERR_NOT_INITIALIZED; \
        }                                       \
        else if (!is_connected)                 \
        {                                       \
            return MODBUS_ERR_NOT_CONNECTED;   \
        }                                       \
    } while (false);

typedef struct
{
    modbus_event_cb_t func;
    void *context;
} event_callback_info_t;

static int sock_fd = -1;
static bool is_initialized = false;
static bool is_connected = false;
static modbus_connect_cb_t connect_callback = NULL;
static void *connect_context = NULL;
static pthread_mutex_t wbuff_lock;
static pthread_t client_tid;
static pthread_mutex_t event_lock;
static event_callback_info_t callbacks[MODBUS_SERVICE_EVENT_ALL];

static bool wait_resp = false;
static pthread_mutex_t resp_data_lock;

cancomm_event_t resp_event_tbl[MODBUS_RESPONSE_MAX] = {
    MODBUS_EVENT_EMETER_OUTPUT,         // MODBUS_RESPONSE_EMETER_OUTPUT
    MODBUS_EVENT_OUTPUT0_ISO_STATUS,    // MODBUS_RESPONSE_OUTPUT0_ISO_STATUS
    MODBUS_EVENT_BUS_STATUS,            // MODBUS_RESPONSE_BUS_STATUS
    MODBUS_EVENT_HV_SELF_TEST_INFO,     // MODBUS_RESPONSE_HV_SELF_TEST_INFO
    MODBUS_EVENT_ALL,                   // MODBUS_RESPONSE_GET_CP0_VERSION
    MODBUS_EVENT_NODE_COMM_LOST,        // MODBUS_RESPONSE_NODE_COMM_LOST
    MODBUS_EVENT_ALL,                   // MODBUS_RESPONSE_GET_EMETER_VERSION
    MODBUS_EVENT_ALL,                   // MODBUS_RESPONSE_GET_ISOCHA_VERSION
    MODBUS_EVENT_GET_EMETER_STATUS,     // MODBUS_RESPONSE_GET_EMETER_STATUS
    MODBUS_EVENT_GET_ISOCHA_STATUS,     // MODBUS_RESPONSE_GET_ISOCHA_STATUS
    MODBUS_EVENT_ALL,                   // MODBUS_RESPONSE_SET_SYSTEM_REBOOT
};

static void read_cb(void);
static modbus_service_err_t modbus_send_request(modbus_request_t request, bool bsync, uint32_t len, uint8_t *data);

static void *client_thread(void *arg)
{
    while (true)
    {
        sleep(RECONNECT_WAIT_TIME);

        SLOGI("%s", "Initialize client");
        sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (sock_fd < 0)
        {
            SLOGE("%s", "Failed to create client socket");
            continue;
        }

        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port   = htons(SOCKET_PORT);

        struct hostent* server;
        server = gethostbyname("127.0.0.1");
        memmove(&addr.sin_addr.s_addr, server->h_addr, server->h_length);

        if (connect(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            SLOGE("%s", "Error connecting to service");
            continue;
        }

        SLOGI("%s", "Service connected");
        is_connected = true;
        pthread_mutex_lock(&event_lock);
        for (uint8_t i = 0; i < MODBUS_EVENT_ALL; i++)
        {
            if (callbacks[i].func)
            {
                SLOGI("Send event %d register request", i);
                Cancomm__Event msg = CANCOMM__EVENT__INIT;
                msg.event = (uint32_t)i;

                uint8_t buff[MSG_BUFF_LENTH_MAX];
                size_t len = cancomm__event__pack(&msg, buff);
                modbus_service_err_t error = modbus_send_request(MODBUS_REQUEST_REGISTER_EVENT, false, len, buff);
                if (error != MODBUS_SERVICE_ERR_NONE)
                {
                    SLOGE("Send event register request failed: %d", error);
                }
            }
        }
        pthread_mutex_unlock(&event_lock);
        connect_callback(is_connected, connect_context);

        read_cb();
        close(sock_fd);
    }

    return NULL;
}

modbus_service_err_t modbus_init(modbus_connect_cb_t connect_cb, void *connect_ctx)
{
    FUNC_ENTRY();

    modbus_service_err_t error = MODBUS_SERVICE_ERR_UNKNOWN;

    if (!is_initialized)
    {
        connect_callback = connect_cb;
        connect_context = connect_ctx;
        memset(callbacks, 0, sizeof(callbacks));

        pthread_mutex_init(&wbuff_lock, NULL);
        pthread_mutex_init(&resp_data_lock, NULL);
        pthread_mutex_init(&event_lock, NULL);

        pthread_attr_t tattr;
        size_t size = 0x20000;//128K
        /* initialized with default attributes */
        pthread_attr_init(&tattr);
        /* setting the size of the stack also */
        pthread_attr_setstacksize(&tattr, size);        
        pthread_create(&client_tid, &tattr, client_thread, NULL);         

        is_initialized = true;
        is_connected = false;
        wait_resp = false;

        error = MODBUS_SERVICE_ERR_NONE;
    }
    else
    {
        error = MODBUS_SERVICE_ERR_ALREADY_INITIALIZED;
    }

    FUNC_EXIT();
    return error;
}

modbus_service_err_t modbus_free(void)
{
    return MODBUS_SERVICE_ERR_NONE;
}

static modbus_service_err_t modbus_send_request(modbus_request_t request, bool bsync, uint32_t len, uint8_t *data)
{
    Rpcproto__ReqHeader req_header = RPCPROTO__REQ_HEADER__INIT;
    req_header.id = request;
    req_header.issync = bsync;
    req_header.has_params = data ? true : false;
    req_header.params.data = data;
    req_header.params.len = len;

    uint8_t *rbuf = NULL;
    size_t rlen = rpc_write_request(&req_header, &rbuf);

    pthread_mutex_lock(&wbuff_lock);
    if (bsync)
    {
        wait_resp = true;
    }
    int32_t write_cnt = write(sock_fd, rbuf, rlen);
    pthread_mutex_unlock(&wbuff_lock);
    free(rbuf);

    if (write_cnt < 0)
    {
        SLOGE("Send request %d failed", request);
        return MODBUS_SERVICE_ERR_SEND_REQ_FAILED;
    }

    // Wait for response from service
    if (bsync)
    {
        uint8_t wait_cnt = 0;
        while (wait_resp && (++wait_cnt < RESPONSE_CHECK_MAX))
        {
            usleep(RESPONSE_CHECK_INTERVAL);
        }

        if (wait_cnt >= RESPONSE_CHECK_MAX)
        {
            SLOGE("Wait response for request %d timed out", request);
            return modbus_service_err_tIMED_OUT;
        }
    }

    return MODBUS_SERVICE_ERR_NONE;
}

static void read_cb(void)
{
    FUNC_ENTRY();

    while (true)
    {
        uint8_t buf[MODBUS_EVENT_ALL * MSG_BUFF_LENTH_MAX];
        uint8_t *buf_ptr = buf;

        size_t read_len = read(sock_fd, buf, sizeof(buf));
        if (read_len <= 0) {
            SLOGI("%s", "Service disconnected");
            is_connected = false;
            connect_callback(is_connected, connect_context);
            break;
        }

        if (read_len < sizeof(uint64_t))
        {
            SLOGE("Read msg len must be at least %d", sizeof(uint64_t));
        }

        while (read_len > 0)
        {
            int32_t rsphdr_data_len = rpc_read_pack_len((const uint8_t *)buf_ptr, read_len);
            if (rsphdr_data_len < 0)
            {
                SLOGE("Error reading rpshdr len %d\n", rsphdr_data_len);
                break;
            }

            Rpcproto__RspHeader *rsp = rpc_read_rsp_data((const uint8_t *)buf_ptr, rsphdr_data_len);
            SLOGI("Modbus response id %lld", rsp->id);

            if (rsp->id >= MODBUS_RESPONSE_MAX)
            {
                SLOGE("%s", "Invalid response id");
                goto CALC_BUFFER;
            }

            modbus_event_t event = resp_event_tbl[rsp->id];
            if (event < MODBUS_EVENT_ALL)
            {
                SLOGI("Modbus event id %d", event);
            }

            pthread_mutex_lock(&event_lock);

            if ((event < MODBUS_EVENT_ALL) && !callbacks[event].func)
            {
                // Event data can be skipped if there's no callback
                SLOGW("No callback assiged for event %d", event);
                goto CALC_BUFFER;
            }

            ProtobufCBinaryData *crsp = &rsp->result;
            switch (rsp->id)
            {

            case MODBUS_RESPONSE_EMETER_OUTPUT:
            {
                ModbusService__EmeterOutput *pack = modbus_service__emeter_output__unpack(NULL, crsp->len, crsp->data);
                modbus_meter_output_t emeter_output;
                emeter_output.m_id = pack->m_id;
                emeter_output.voltage = pack->voltage;
                emeter_output.current = pack->current;
                emeter_output.power = pack->power;
                callbacks[event].func(event, (void *)&rec_output, callbacks[event].context);
                modbus_service__emeter_output__free_unpacked(pack, NULL);
                break;
            }

            case MODBUS_RESPONSE_ISO_STATUS:
            {
                ModbusService__IsochaIsoStatus *pack = modbus_service__isocha_iso_status__unpack(NULL, crsp->len, crsp->data);
                modbus_iso_status_t iso_status = (uint8_t)pack->status;
                callbacks[event].func(event, (void *)&iso_status, callbacks[event].context);
                modbus_service__isocha_iso_status__free_unpacked(pack, NULL);
                break;
            }

            case MODBUS_RESPONSE_NODE_COMM_LOST:
            {
                ModbusService__NodeCommLost *pack = modbus_service__node_comm_lost__unpack(NULL, crsp->len, crsp->data);
                callbacks[event].func(event, (void *)&pack->node, callbacks[event].context);
                modbus_service__node_comm_lost__free_unpacked(pack, NULL);
                break;
            }

            case MODBUS_RESPONSE_GET_EMETER_VERSION:
            {
                ModbusService__Version *pack = modbus_service__version__unpack(NULL, crsp->len, crsp->data);
                modbus_version_t *emeter_version = emeter_version_local;
                pthread_mutex_lock(&resp_data_lock);
                strncpy(emeter_version->major, pack->major, MODBUS_BOARD_INFO_MAX);
                strncpy(emeter_version->minor, pack->minor, MODBUS_BOARD_INFO_MAX);
                strncpy(emeter_version->subminor, pack->subminor, MODBUS_BOARD_INFO_MAX);
                wait_resp = false;
                pthread_mutex_unlock(&resp_data_lock);
                modbus_service__version__free_unpacked(pack, NULL);
                break;
            } 

            case MODBUS_RESPONSE_GET_ISOCHA_VERSION:
            {
                ModbusService__Version *pack = modbus_service__version__unpack(NULL, crsp->len, crsp->data);
                modbus_version_t *isocha_version = isocha_version_local;
                pthread_mutex_lock(&resp_data_lock);
                strncpy(isocha_version->major, pack->major, MODBUS_BOARD_INFO_MAX);
                strncpy(isocha_version->minor, pack->minor, MODBUS_BOARD_INFO_MAX);
                strncpy(isocha_version->subminor, pack->subminor, MODBUS_BOARD_INFO_MAX);
                wait_resp = false;
                pthread_mutex_unlock(&resp_data_lock);
                modbus_service__version__free_unpacked(pack, NULL);
                break;
            } 

        case MODBUS_RESPONSE_GET_EMETER_STATUS:
            {
                ModbusService__EmeterError *pack = modbus_service__emeter_error__unpack(NULL, crsp->len, crsp->data);
                uint32_t *emeter_error_code = emeter_error_code_local;
                pthread_mutex_lock(&resp_data_lock);
                if (*emeter_error_code != pack->error_code)
                    *emeter_error_code = pack->error_code;
                wait_resp = false;
                pthread_mutex_unlock(&resp_data_lock);
                modbus_service__emeter_error__free_unpacked(pack, NULL);
                break;
            } 

            case MODBUS_RESPONSE_GET_ISOCHA_STATUS:
            {
                ModbusService__IsochaIsoStatus *pack = modbus_service__isocha_error__unpack(NULL, crsp->len, crsp->data);
                uint32_t *isocha_error_code = isocha_error_code_local;
                pthread_mutex_lock(&resp_data_lock);
                if (*isocha_error_code != pack->error_code)
                    *isocha_error_code = pack->error_code;
                wait_resp = false;
                pthread_mutex_unlock(&resp_data_lock);
                modbus_service__isocha_error__free_unpacked(pack, NULL);
                break;
            } 

        case MODBUS_RESPONSE_SET_SYSTEM_REBOOT:
            {
                // There will be no feedback for system reboot request
                break;
            } 

            default:
                SLOGE("Unexpected event %d", rsp->id);
                break;
            }

        CALC_BUFFER:
            pthread_mutex_unlock(&event_lock);
            rpc_clear_rsp(rsp);

            {
                // Update buffer parsing params
                int32_t rsp_len = rsphdr_data_len + sizeof(uint64_t);
                read_len -= rsp_len;
                buf_ptr += rsp_len;
                if (buf_ptr >= (buf + MODBUS_EVENT_ALL * MSG_BUFF_LENTH_MAX))
                {
                    SLOGE("Buffer Event Overflow:%d", (int32_t)(buf_ptr - buf));
                    break;
                }
            }
        }
    }

    FUNC_EXIT();
}

modbus_service_err_t modbus_register_event(modbus_event_t event, modbus_event_cb_t func_ptr, void *context)
{
    SLOGI("Register event %d", event);

    modbus_service_err_t error = MODBUS_SERVICE_ERR_UNKNOWN;
    if (!is_initialized)
    {
        error = MODBUS_SERVICE_ERR_NOT_INITIALIZED;
    }

    if (!func_ptr)
    {
        error = MODBUS_SERVICE_ERR_INVALID_PARAM;
    }
    else
    {
        pthread_mutex_lock(&event_lock);
        if (event < MODBUS_EVENT_ALL)
        {
            callbacks[event].func = func_ptr;
            callbacks[event].context = context;
        }
        else
        {
            for (uint8_t i = 0; i < MODBUS_EVENT_ALL; i++)
            {
                callbacks[i].func = func_ptr;
                callbacks[i].context = context;
            }
        }
        pthread_mutex_unlock(&event_lock);

        if (is_connected)
        {
            ModbusService__Event msg = MODBUS_SERVICE__EVENT__INIT;
            msg.event = (uint32_t)event;

            uint8_t buff[MSG_BUFF_LENTH_MAX];
            size_t len = modbus_service__event__pack(&msg, buff);
            error = modbus_send_request(MODBUS_REQUEST_REGISTER_EVENT, false, len, buff);
        }
        else
        {
            SLOGI("Event %d register request not sent", event);
        }

        error = MODBUS_SERVICE_ERR_NONE;
    }

    return error;
}

modbus_service_err_t modbus_unregister_event(cancomm_event_t event)
{
    SLOGI("Unregister event %d", event);

    CHECK_CLIENT_STATUS();

    pthread_mutex_lock(&event_lock);
    if (event < MODBUS_EVENT_ALL)
    {
        callbacks[event].func = NULL;
        callbacks[event].context = NULL;
    }
    else
    {
        for (uint8_t i = 0; i < MODBUS_EVENT_ALL; i++)
        {
            callbacks[i].func = NULL;
            callbacks[i].context = NULL;
        }
    }
    pthread_mutex_unlock(&event_lock);

    ModbusService__Event msg = MODBUS_SERVICE__EVENT__INIT;
    msg.event = (uint32_t)event;

    uint8_t buff[MSG_BUFF_LENTH_MAX];
    size_t len = modbus_service__event__pack(&msg, buff);
    modbus_service_err_t error = modbus_send_request(MODBUS_REQUEST_UNREGISTER_EVENT, false, len, buff);

    return error;
}

modbus_service_err_t modbus_request_emeter_output(void)
{
    FUNC_ENTRY();
    SLOGI("Get emeter%d output", m_id);

    CHECK_CLIENT_STATUS();

    modbus_service_err_t error = MODBUS_SERVICE_ERR_UNKNOWN;
    if (m_id >= 1)
    {
        SLOGE("Invalid rec id %d", m_id);
        error = MODBUS_SERVICE_ERR_INVALID_PARAM;
    }
    else
    {
        modbus_request_t request = MODBUS_REQUEST_EMETER_OUTPUT;
        modbus_service_err_t error = modbus_send_request(request, true, 0, NULL);
        if (error != MODBUS_SERVICE_ERR_NONE)
        {
            SLOGE("Get emeter output failed with error %d", error);
        }
    }

    FUNC_EXIT();
    return error;
}

static modbus_version_t emeter_verision_info_local;

modbus_service_err_t cancomm_get_emeter_version_infor(modbus_version_t *emeter_version)
{
    FUNC_ENTRY();

    CHECK_CLIENT_STATUS();

    if (!emeter_version)
    {
        return MODBUS_SERVICE_ERR_INVALID_PARAM;
    }

    modbus_request_t request = MODBUS_REQUEST_GET_EMETER_VERSION;
    modbus_service_err_t error = modbus_send_request(request, true, 0, NULL);
    if (error != MODBUS_SERVICE_ERR_NONE)
    {
        SLOGE("Get Emeter version failed with error %d", error);
    }
    else
    {
        pthread_mutex_lock(&resp_data_lock);
        *emeter_version = emeter_verision_info_local;
        pthread_mutex_unlock(&resp_data_lock);
    }

    FUNC_EXIT();
    return error;
}


static modbus_version_t isocha_verision_info_local;

modbus_service_err_t cancomm_get_isocha_version_infor(modbus_version_t *isocha_version)
{
    FUNC_ENTRY();

    CHECK_CLIENT_STATUS();

    if (!emeter_version)
    {
        return MODBUS_SERVICE_ERR_INVALID_PARAM;
    }

    modbus_request_t request = MODBUS_REQUEST_GET_ISOCHA_VERSION;
    modbus_service_err_t error = modbus_send_request(request, true, 0, NULL);
    if (error != MODBUS_SERVICE_ERR_NONE)
    {
        SLOGE("Get Emeter version failed with error %d", error);
    }
    else
    {
        pthread_mutex_lock(&resp_data_lock);
        *isocha_version = isocha_verision_info_local;
        pthread_mutex_unlock(&resp_data_lock);
    }

    FUNC_EXIT();
    return error;
}

static uint32_t  emeter_error_code_local;
modbus_service_err_t cancomm_get_emeter_error_code(uint32_t *emeter_error_code)
{
    FUNC_ENTRY();

    CHECK_CLIENT_STATUS();

    if ((!hv_output_error) || (idx > 2))
    {
        return CANCOMM_ERR_INVALID_PARAM;
    }

    modbus_request_t request = MODBUS_REQUEST_GET_ISOCHA_STATUS;
    modbus_service_err_t error = modbus_send_request(request, true, 0, NULL);
    if (error != MODBUS_SERVICE_ERR_NONE)
    {
        SLOGE("Get Hv error code failed with error %d", error);
    }
    else
    {
        pthread_mutex_lock(&resp_data_lock);
        *emeter_error_code = emeter_error_code_local;
        pthread_mutex_unlock(&resp_data_lock);
    }

    FUNC_EXIT();
    return error;
}

static uint32_t  isocha_error_code_local;
modbus_service_err_t cancomm_get_isocha_error_code(uint32_t *isocha_error_code)
{
    FUNC_ENTRY();

    CHECK_CLIENT_STATUS();

    if ((!hv_output_error) || (idx > 2))
    {
        return CANCOMM_ERR_INVALID_PARAM;
    }

    modbus_request_t request = MODBUS_REQUEST_GET_ISOCHA_STATUS;
    modbus_service_err_t error = modbus_send_request(request, true, 0, NULL);
    if (error != MODBUS_SERVICE_ERR_NONE)
    {
        SLOGE("Get Hv error code failed with error %d", error);
    }
    else
    {
        pthread_mutex_lock(&resp_data_lock);
        *isocha_error_code = isocha_error_code_local;
        pthread_mutex_unlock(&resp_data_lock);
    }

    FUNC_EXIT();
    return error;
}

modbus_service_err_t cancomm_set_system_reboot(void)
{
    FUNC_ENTRY();
    CHECK_CLIENT_STATUS();
    modbus_service_err_t error = cancomm_send_request(CANCOMM_REQUEST_SET_SYSTEM_REBOOT, false, 0, NULL);
    FUNC_EXIT();
    return error;
}

modbus_service_err_t modbus_set_iso_measurement_state(bool state)
{
    SLOGI("Isolation Measurement state %d", state);

    CHECK_CLIENT_STATUS();

    modbus_request_t request = MODBUS_REQUEST_SET_ISO_MEASURE_STATE;

    ModbusService__IsochaMeasureControl msg = MODBUS_SERVICE__ISOCHA_MEASURE_CONTROL__INIT;
    msg.start = (bool)state;

    uint8_t buff[MSG_BUFF_LENTH_MAX];
    size_t len = modbus_service__isocha_measure_control__pack(&msg, buff);
    modbus_service_err_t error = modbus_send_request(request, false, len, buff);
    if (error != MODBUS_SERVICE_ERR_NONE)
    {
        SLOGE("Set Isolation measurement state failed with error %d", error);
    }

    return error;
}