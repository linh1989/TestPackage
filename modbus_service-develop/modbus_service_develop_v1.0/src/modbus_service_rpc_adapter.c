
#define LOG_TAG "modbus_service_rpc_adapter"

#include "modbus_service_rpc_adapter.h"

#include <malloc.h>
#include "logger.h"
#include "rpc.h"

#include "modbus_service_rpc_adapter.h"

/* Rpcproto method declarations */
static int register_event(ProtobufCBinaryData *req, ProtobufCBinaryData *reply, void *context);
static int unregister_event(ProtobufCBinaryData *req, ProtobufCBinaryData *reply, void *context);
static int get_emeter_output(ProtobufCBinaryData *req, ProtobufCBinaryData *reply, void *context);
static int get_emeter_version(ProtobufCBinaryData *req, ProtobufCBinaryData *reply, void *context);
static int get_isocha_version(ProtobufCBinaryData *req, ProtobufCBinaryData *reply, void *context);
static int get_emeter_status(ProtobufCBinaryData *req, ProtobufCBinaryData *reply, void *context);
static int get_isocha_status(ProtobufCBinaryData *req, ProtobufCBinaryData *reply, void *context);
static int set_system_reboot(ProtobufCBinaryData *req, ProtobufCBinaryData *reply, void *context);
static int set_iso_measurement_state(ProtobufCBinaryData *req, ProtobufCBinaryData *reply, void *context);
/* End of method declarations */

/* Rpcproto methods table */
rpc2_handler_func vtable[MODBUS_REQUEST_MAX] = {
    [MODBUS_REQUEST_REGISTER_EVENT] = register_event,
    [MODBUS_REQUEST_UNREGISTER_EVENT] = unregister_event,
    [MODBUS_REQUEST_EMETER_OUTPUT] = get_emeter_output,
    [MODBUS_REQUEST_GET_EMETER_VERSION] = get_emeter_version,
    [MODBUS_REQUEST_GET_ISOCHA_VERSION] = get_isocha_version,
    [MODBUS_REQUEST_GET_EMETER_STATUS] = get_emeter_status,
    [MODBUS_REQUEST_GET_ISOCHA_STATUS] = get_isocha_status,
    [MODBUS_REQUEST_SET_SYSTEM_REBOOT] = set_system_reboot,
    [MODBUS_REQUEST_SET_ISO_MEASURE_STATE] = set_iso_measurement_state,
};

modbus_err_t rpc_invoke_call(Rpcproto__ReqHeader *reqhdr, Rpcproto__RspHeader *rsphdr, void *context)
{
    switch (reqhdr->id)
    {
    case MODBUS_REQUEST_EMETER_OUTPUT:
        rsphdr->id = MODBUS_RESPONSE_EMETER_OUTPUT;
        break;

    case MODBUS_REQUEST_GET_EMETER_VERSION:
        rsphdr->id = MODBUS_RESPONSE_GET_EMETER_VERSION;
        break;

    case MODBUS_REQUEST_GET_ISOCHA_VERSION:
        rsphdr->id = MODBUS_RESPONSE_GET_ISOCHA_VERSION;
        break;

    case MODBUS_REQUEST_GET_EMETER_STATUS:
        rsphdr->id = MODBUS_RESPONSE_GET_EMETER_STATUS;
        break;

    case MODBUS_REQUEST_GET_ISOCHA_STATUS:
        rsphdr->id = MODBUS_RESPONSE_GET_ISOCHA_STATUS;
        break;

    case MODBUS_REQUEST_SET_ISO_MEASURE_STATE:
        rsphdr->id = MODBUS_RESPONSE_SET_ISO_MEASURE_STATE;
        break;

    default:
        rsphdr->id = reqhdr->id;
        break;
    }
    
    modbus_err_t err = (modbus_err_t)vtable[reqhdr->id](&reqhdr->params, &rsphdr->result, context);

    return err;
}

static int register_event(ProtobufCBinaryData *req, ProtobufCBinaryData *reply, void *context)
{
    ModbusService__Event *creq = modbus_service__event__unpack(NULL, req->len, req->data);
    connection_data_t *conn_data = (connection_data_t *)context;
    const modbus_impl_handle_t* handler = modbus_impl_get_handle();
    if (creq->event != MODBUS_EVENT_ALL)
    {
        conn_data->receiver->register_event[creq->event] = true;
        handler->register_event(creq->event);
    }
    else
    {
        for (uint8_t i = 0; i < MODBUS_EVENT_ALL; i++)
        {
            conn_data->receiver->register_event[i] = true;
            handler->register_event(creq->event);
        }
    }
    return MODBUS_ERR_NONE;
}

static int unregister_event(ProtobufCBinaryData *req, ProtobufCBinaryData *reply, void *context)
{
    ModbusService__Event *creq = modbus_service__event__unpack(NULL, req->len, req->data);
    connection_data_t *conn_data = (connection_data_t *)context;
    if (creq->event != MODBUS_EVENT_ALL)
    {
        conn_data->receiver->register_event[creq->event] = false;
    }
    else
    {
        for (uint8_t i = 0; i < MODBUS_EVENT_ALL; i++)
        {
            conn_data->receiver->register_event[i] = false;
        }
    }
    return MODBUS_ERR_NONE;
}

static int get_emeter_output(ProtobufCBinaryData *req, ProtobufCBinaryData *reply, void *context)
{
    modbus_meter_output_t emeter_output;
    modbus_err_t error = modbus_impl_get_handle()->get_emeter_output(&emeter_output);
    if (error != MODBUS_ERR_NONE)
    {
        SLOGE("Get emeter output info error %d", error);
    }
    else
    {
        uint32_t m_id;
        uint32_t voltage;
        uint32_t current;
        uint32_t power;

        m_id = (uint32_t)emeter_output.m_id;
        voltage = (uint32_t)emeter_output.voltage;
        current = (uint32_t)emeter_output.current;
        power = (uint32_t)emeter_output.power;

        ModbusService__EmeterOutput pack = MODBUS_SERVICE__EMETER_OUTPUT__INIT;
        pack.m_id = m_id;
        pack.voltage = voltage;
        pack.current = current;
        pack.power = power;

        reply->len = modbus_service__emeter_output__get_packed_size(&pack);
        reply->data = (uint8_t *)malloc(reply->len);

        modbus_service__emeter_output__pack(&pack, reply->data);
    }
    return error;
}

static int get_emeter_version(ProtobufCBinaryData *req, ProtobufCBinaryData *reply, void *context)
{
    modbus_version_t temp_version;
    modbus_err_t error = modbus_impl_get_handle()->get_emeter_version_info(&temp_version);
    if (error != MODBUS_ERR_NONE)
    {
        SLOGE("Get temp emeter version info error %d", error);
    }
    else
    {
        uint32_t temp_major;
        uint32_t temp_minor;
        uint32_t temp_subminor;

        temp_major = temp_version.major;
        temp_minor = temp_version.minor;
        temp_subminor = temp_version.subminor;

        ModbusService__Version pack = MODBUS_SERVICE__VERSION__INIT;
        pack.major = temp_major;
        pack.minor = temp_minor;
        pack.subminor = temp_subminor;

        reply->len = modbus_service__version__get_packed_size(&pack);
        reply->data = (uint8_t *)malloc(reply->len);

        modbus_service__version__pack(&pack, reply->data);
    }
    return error;
}

static int get_isocha_version(ProtobufCBinaryData *req, ProtobufCBinaryData *reply, void *context)
{
    modbus_version_t temp_version;
    modbus_err_t error = modbus_impl_get_handle()->get_isocha_version_info(&temp_version);
    if (error != MODBUS_ERR_NONE)
    {
        SLOGE("Get temp emeter version info error %d", error);
    }
    else
    {
        uint32_t temp_major;
        uint32_t temp_minor;
        uint32_t temp_subminor;

        temp_major = temp_version.major;
        temp_minor = temp_version.minor;
        temp_subminor = temp_version.subminor;

        ModbusService__Version pack = MODBUS_SERVICE__VERSION__INIT;
        pack.major = temp_major;
        pack.minor = temp_minor;
        pack.subminor = temp_subminor;

        reply->len = modbus_service__version__get_packed_size(&pack);
        reply->data = (uint8_t *)malloc(reply->len);

        modbus_service__version__pack(&pack, reply->data);
    }
    return error;
}

static int get_emeter_status(ProtobufCBinaryData *req, ProtobufCBinaryData *reply, void *context)
{
    unit32_t temp_info;
    modbus_err_t error = modbus_impl_get_handle()->get_emeter_status_info(&temp_info);
    if (error != MODBUS_ERR_NONE)
    {
        SLOGE("Get temp emeter version info error %d", error);
    }
    else
    {
        uint32_t temp_status;

        temp_status = temp_info.status;

        ModbusService__EmeterError pack = MODBUS_SERVICE__EMETER_ERROR__INIT;
        pack.status = temp_status;

        reply->len = modbus_service__emeter_error__get_packed_size(&pack);
        reply->data = (uint8_t *)malloc(reply->len);

        modbus_service__emeter_error__pack(&pack, reply->data);
    }
    return error;
}

static int get_isocha_status(ProtobufCBinaryData *req, ProtobufCBinaryData *reply, void *context)
{
    unit32_t temp_info;
    modbus_err_t error = modbus_impl_get_handle()->get_isocha_status_info(&temp_info);
    if (error != MODBUS_ERR_NONE)
    {
        SLOGE("Get temp emeter version info error %d", error);
    }
    else
    {
        uint32_t temp_status;

        temp_status = temp_info.status;

        ModbusService__IsochaError pack = MODBUS_SERVICE__ISOCHA_ERROR__INIT;
        pack.status = temp_status;

        reply->len = modbus_service__isocha_error__get_packed_size(&pack);
        reply->data = (uint8_t *)malloc(reply->len);

        modbus_service__isocha_error__pack(&pack, reply->data);
    }
    return error;
}

static int set_iso_measurement_state(ProtobufCBinaryData *req, ProtobufCBinaryData *reply, void *context)
{
    ModbusService__IsochaMeasureControl *creq = modbus_service__isocha_measure_control__unpack(NULL, req->len, req->data);
    modbus_impl_get_handle()->set_isocha_measurement_state((bool)creq->start);
    return MODBUS_ERR_NONE;
}

static int set_system_reboot(ProtobufCBinaryData *req, ProtobufCBinaryData *reply, void *context)
{
    modbus_impl_get_handle()->set_system_reboot();
    return MODBUS_ERR_NONE;
}
