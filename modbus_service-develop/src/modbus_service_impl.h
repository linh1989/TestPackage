/****************************************************
 *  modbus_service_impl.h
 *  Created on: 03-May-2025 20:55:54
 *  Implementation of the Class modbus_service_impl
 *  Original author: LinhNM
 ****************************************************/

#ifndef _MODBUS_SERVICE_IMPL_H_
#define _MODBUS_SERVICE_IMPL_H_

#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include "modbus_service_types.h"
#include "modbus_service_client.h"

typedef struct {
    modbus_service_err_t (*run)                       ();
	modbus_service_err_t (*register_event)			  (modbus_event_t event);
    modbus_service_err_t (*get_emeter_output)         (modbus_meter_output_t* emeter_output);
	modbus_service_err_t (*get_emeter_version_info)   (modbus_version_t* version);
	modbus_service_err_t (*get_isocha_version_info)   (modbus_version_t* version);
	modbus_service_err_t (*get_emeter_status_info)    (uint32_t* error_code);
	modbus_service_err_t (*get_isocha_status_info)    (uint32_t* error_code);
	modbus_service_err_t (*set_system_reboot)         ();
	modbus_service_err_t (*set_isocha_measurement_state)   		      (bool start);
} modbus_impl_handle_t;

typedef void (*notification_handler_t)(modbus_response_t response, void* data, void* context);

typedef struct {
	void* ctx;
	void (*on_notify)(modbus_response_t response, void* data, void* context);
    bool register_event[MODBUS_EVENT_ALL];		
} modbus_impl_receiver;

typedef struct {
	int  (*register_receiver)  (modbus_impl_receiver*);
	int  (*unregister_receiver)(modbus_impl_receiver*);
	void (*notify)             (modbus_response_t response, void* context);
} modbus_impl_subject;

typedef struct {
	int client_fd;
	modbus_impl_receiver* receiver;
} connection_data_t;

const modbus_impl_handle_t* modbus_impl_get_handle(void);
modbus_impl_subject*  modbus_impl_get_subject(void);
modbus_impl_receiver* modbus_impl_create_receiver(void* context, notification_handler_t ntfy);
bool modbus_get_connect_status(modbus_module_t i);
int modbus_feed_event(void* data, size_t data_len);

#endif // _MODBUS_SERVICE_IMPL_H_
