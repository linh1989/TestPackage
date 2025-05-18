
#ifndef _MODBUS_SERVICE_RPC_ADAPTER_H_
#define _MODBUS_SERVICE_RPC_ADAPTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rpc.h"
#include "protobuf-c/rpc.pb-c.h"

#include "protobuf-c/modbus_service.pb-c.h"
#include "modbus_service_types.h"
#include "modbus_service_client.h"

modbus_service_err_t rpc_invoke_call(Rpcproto__ReqHeader* reqhdr, Rpcproto__RspHeader* rsphdr, void* context);

#ifdef __cplusplus
}
#endif

#endif // _MODBUS_SERVICE_RPC_ADAPTER_H_
