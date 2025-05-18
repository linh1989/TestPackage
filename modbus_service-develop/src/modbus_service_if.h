/****************************************************
 *  modbus_service_if.h
 *  Created on: 18-May-2025 23:29:55
 *  Implementation of the Interface modbus_service_if
 *  Original author: linhnm
 ****************************************************/

#ifndef _MODBUS_SERVICE_IF_H_
#define _MODBUS_SERVICE_IF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "modbus_service_types.h"

modbus_service_err_t modbus_init(modbus_connect_cb_t connect_cb, void* connect_ctx);
modbus_service_err_t modbus_free(void);
modbus_service_err_t modbus_register_event(modbus_event_t event, modbus_event_cb_t event_cb, void* event_ctx);
modbus_service_err_t modbus_unregister_event(cmodbus_event_t event);

modbus_service_err_t modbus_request_emeter_output(void);
modbus_service_err_t cancomm_get_emeter_version_infor(modbus_version_t *emeter_version);
modbus_service_err_t cancomm_get_isocha_version_infor(modbus_version_t *isocha_version);
modbus_service_err_t cancomm_get_emeter_error_code(uint32_t *emeter_error_code);
modbus_service_err_t cancomm_get_isocha_error_code(uint32_t *isocha_error_code);

modbus_service_err_t modbus_set_system_reboot(void);
modbus_service_err_t modbus_set_iso_measurement_state(bool state);

#ifdef __cplusplus
}
#endif

#endif // _MODBUS_SERVICE_IF_H_
