
#ifndef _MODBUS_SERVICE_INTF_H_
#define _MODBUS_SERVICE_INTF_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <modbus.h>

#include "modbus_service_types.h"


typedef enum {
    MODBUS_PROTOCOL_BCM = 0,
    MODBUS_PROTOCOL_TCP,
    MODBUS_PROTOCOL_RTU,
    MODBUS_PROTOCOL_TYPE_MAX
} modbus_protocol_t;

#define DEVICE      "/dev/ttyUSB0"
#define BAUD        19200
#define PARITY      'N'
#define DATA_BIT    8
#define STOP_BIT    1

#define REG_ADDRESS 0x0001
#define NB_REGS     1

void modbus_intf_init(void);
void modbus_intf_deinit(void);

modbus_err_t modbus_intf_read_request(modbus_t *ctx, int slave_id, uint32_t reg_address, uint32_t* read_value);
modbus_err_t modbus_intf_write_request(modbus_t *ctx, int slave_id, uint32_t reg_address, uint32_t write_value);

#endif // _MODBUS_SERVICE_INTF_H_
