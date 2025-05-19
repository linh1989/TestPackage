/****************************************************
 *  modbus_service_client.h
 *  Created on: 15-May-2025 16:27:24
 *  Implementation of the Class modbus_service_client.c
 *  Original author: linhnm
 ****************************************************/

#ifndef _MODBUS_SERVICE_CLIENT_H_
#define _MODBUS_SERVICE_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "modbus_service_types.h"

typedef enum {
    MODBUS_REQUEST_REGISTER_EVENT = 0,
    MODBUS_REQUEST_UNREGISTER_EVENT,
    MODBUS_REQUEST_EMETER_OUTPUT,
	MODBUS_REQUEST_GET_EMETER_VERSION,
	MODBUS_REQUEST_GET_ISOCHA_VERSION,
	MODBUS_REQUEST_GET_EMETER_STATUS,
	MODBUS_REQUEST_GET_ISOCHA_STATUS,
	MODBUS_REQUEST_SET_SYSTEM_REBOOT,
	MODBUS_REQUEST_SET_ISO_MEASURE_STATE,
    MODBUS_REQUEST_MAX
} modbus_request_t;

typedef enum {
    MODBUS_RESPONSE_EMETER_OUTPUT = 0,
	MODBUS_RESPONSE_ISO_STATUS,
	MODBUS_RESPONSE_NODE_COMM_LOST,
	MODBUS_RESPONSE_GET_EMETER_VERSION,
	MODBUS_RESPONSE_GET_ISOCHA_VERSION,
	MODBUS_RESPONSE_GET_EMETER_STATUS,
	MODBUS_RESPONSE_GET_ISOCHA_STATUS,
	MODBUS_RESPONSE_SET_SYSTEM_REBOOT,
	MODBUS_RESPONSE_SET_ISO_MEASURE_STATE,
    MODBUS_RESPONSE_MAX
} modbus_response_t;

extern modbus_event_t resp_event_tbl[MODBUS_RESPONSE_MAX];
extern modbus_response_t event_resp_tbl[MODBUS_EVENT_ALL];

#ifdef __cplusplus
}
#endif

#endif // _MODBUS_SERVICE_CLIENT_H_
