
#ifndef _MODBUS_SERVICE_H_
#define _MODBUS_SERVICE_H_

#include "modbus_service_types.h"


#define SOCKET_PORT 12345

modbus_err_t modbus_service_run(const char* service);

#endif // _MODBUS_SERVICE_H_
