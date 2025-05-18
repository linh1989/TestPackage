
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
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/isotp.h>

#include "can_message.h"
#include "cancomm_types.h"

typedef enum {
    MODBUS_MNGR_RAW_REC = 0,
    MODBUS_MNGR_RAW_SYSTEM,
    MODBUS_MNGR_ISOTP_ARQ_HV,
    MODBUS_MNGR_MAX
} can_mngr_id_t;

typedef enum {
    MODBUS_PROTOCOL_BCM = 0,
    MODBUS_PROTOCOL_TCP,
    MODBUS_PROTOCOL_RTU,
    MODBUS_PROTOCOL_TYPE_MAX
} modbus_protocol_t;

typedef struct {
    modbus_mngr_id_t       id; 
    int32_t             socket;
    modbus_protocol_t      protocol;
    uint16_t            channel;
    uint32_t            tx_id;
    uint32_t            rx_id;
} modbus_mngr_t;

void modbus_intf_init(void);
void modbus_intf_deinit(void);

can_mngr_t* modbus_get_mngr(can_mngr_id_t mngr_id);

void modbus_intf_tx_control(bool is_enable);

modbus_service_err_t modbus_intf_raw_recv(can_mngr_id_t mngr_id, can_msg_id_t* msg_id, uint8_t* msg_data);
modbus_service_err_t modbus_intf_raw_send(can_msg_id_t  msg_id);


#endif // _MODBUS_SERVICE_INTF_H_
