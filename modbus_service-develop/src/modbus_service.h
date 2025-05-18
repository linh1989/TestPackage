
#ifndef _MODBUS_SERVICE_H_
#define _MODBUS_SERVICE_H_

#include "modbus_service_types.h"


#define SOCKET_PORT 9876

modbus_service_err_t modbus_service_run(const char* service);

#endif // _MODBUS_SERVICE_H_
