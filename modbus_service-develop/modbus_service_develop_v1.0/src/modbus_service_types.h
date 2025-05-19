/****************************************************
 *  modbus_service_types.h
 *  Created on: 15-May-2025 16:27:24
 *  Implementation of the Enumeration modbus_service_type
 *  Original author: linhnm
 ****************************************************/

#ifndef _MODBUS_TYPES_H_
#define _MODBUS_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#define MODBUS_DEVICE_NUM					   2
#define MODBUS_BOARD_INFO_MAX                  128
// MODBUS_SERVICE error enumeration
typedef enum {
	MODBUS_ERR_NONE = 0,
	MODBUS_ERR_NOT_INITIALIZED,
	MODBUS_ERR_ALREADY_INITIALIZED,
	MODBUS_ERR_NOT_CONNECTED,
	MODBUS_ERR_MALLOC_FAILED,
	MODBUS_ERR_INVALID_PARAM,
	MODBUS_ERR_SEND_REQ_FAILED,
	MODBUS_ERR_INVALID_RECEIVED_MSG,
	MODBUS_ERR_SEND_MSG_FAILED,
	MODBUS_ERR_TIMED_OUT,
	MODBUS_ERR_NO_UPDATE,
	MODBUS_ERR_UNKNOWN,
	MODBUS_ERR_MAX
} modbus_err_t;

// modbus_service event enumeration
typedef enum {
	MODBUS_EVENT_EMETER_OUTPUT = 0,
	MODBUS_EVENT_ISO_STATUS,
	MODBUS_EVENT_NODE_COMM_LOST,
	MODBUS_EVENT_GET_EMETER_VERSION,
	MODBUS_EVENT_GET_ISOCHA_VERSION,
	MODBUS_EVENT_GET_EMETER_STATUS,
	MODBUS_EVENT_GET_ISOCHA_STATUS,
	MODBUS_EVENT_SET_SYSTEM_REBOOT,
	MODBUS_EVENT_SET_ISO_MEASURE_STATE,
	MODBUS_EVENT_ALL
} modbus_event_t;

// Callback prototypes
typedef void (*modbus_connect_cb_t)(bool is_connected, void* context);
typedef void (*modbus_event_cb_t)  (modbus_event_t event, void* data, void* context);

#define CANPRD_MAX_CHECK (20U)

typedef enum
{
	EMETER_MODULE = 0,
	ISOCHA_MODULE,
	NUMBER_OF_MODULE,
} modbus_module_t;

typedef enum
{    
	MODBUS_LOST_CONNECT = 0U,
	MODBUS_CONNECTED = 1U,
} modbus_connection_status_t;


typedef struct {
	uint8_t major;
	uint8_t minor;
	uint8_t subminor;
} modbus_version_t;

typedef struct {
	uint32_t m_id;
	uint32_t voltage;
	uint32_t current;
	uint32_t power;
} modbus_meter_output_t;

typedef enum
{
	MODBUS_SELTEST_OK = 0,
	MODBUS_SELTEST_NG = 1,
	MODBUS_SELTEST_PROCESSING = 2,
} modbus_selftest_state_t;

typedef enum {
	ISOCHA_ISO_INVALID = 0,
	ISOCHA_ISO_VALID,	
	ISOCHA_ISO_WARNING,
	ISOCHA_ISO_FAULT,
	ISOCHA_ISO_MAX
} modbus_iso_status_t;

typedef struct {
	uint32_t error_code;
} modbus_error_t;

typedef enum {
	MODBUS_NODE_EMETER  = 0x01,
	MODBUS_NODE_ISOCHA  = 0x03,
} modbus_nodes_t;

#ifdef __cplusplus
}
#endif

#endif // _MODBUS_TYPES_H_
