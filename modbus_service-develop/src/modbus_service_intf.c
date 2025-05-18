/****************************************************
 *  modbus_service_intf.c
 *  Created on: 03-Jun-2025 9:24:04 AM
 *  Implementation of the Class modbus_service_intf
 *  Original author: linhnm
 ****************************************************/

#define LOG_TAG "modbus_service_intf"

#include "modbus_service_intf.h"

#include <pthread.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <sys/select.h>

#include "can_utils.h"
#include "logger.h"

#define MODBUS_DEV_NUM                       2
#define MODBUS_DEV_NAME_LEN                  10

#define MODBUS_SOCKET_ADDR_OPT               0x10

#define MODBUS_ADDRESS_EMTER                 0x01
#define MODBUS_ADDRESS_ISOCHA                0x03

static pthread_mutex_t modbus_mutex[CAN_DEV_NUM];

static bool is_enable_tx = false;

void modbus_service_intf_init(void)
{
    FUNC_ENTRY();

    // Init mutexes for CAN channels
    for (uint8_t i = 0; i < CAN_DEV_NUM; i++) {
        pthread_mutex_init(&can_mutex[i], NULL);
    }

    // Init CAN managers
    for (uint8_t i = 0; i < CAN_MNGR_MAX; i++) {
        can_mngr_t* can_mngr = &can_mngr_tbl[i];

        can_mngr->socket = socket(PF_CAN, socket_type[can_mngr->protocol], socket_protocol[can_mngr->protocol]);
        if (can_mngr->socket < 0) {
            SLOGE("Unable to open CAN socket Id %d", can_mngr->id);
            continue;
        }

        struct ifreq ifr;
        strncpy(ifr.ifr_name, can_interface[can_mngr->channel], CAN_DEV_NAME_LEN);
        ifr.ifr_ifindex = if_nametoindex(ifr.ifr_name);
        if (!ifr.ifr_ifindex) {
            SLOGE("Unable to set ifr name for CAN socket Id %d", can_mngr->id);
            continue;
        }

        struct sockaddr_can addr;
        memset((void*)&addr, 0, sizeof(addr));
        addr.can_family  = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;

        if (can_mngr->protocol != CAN_PROTOCOL_RAW) {
            addr.can_addr.tp.tx_id = can_mngr->tx_id;
            addr.can_addr.tp.rx_id = can_mngr->rx_id;

            struct can_isotp_options opts;
            struct can_isotp_fc_options fcopts;
            memset(&opts, 0, sizeof(opts));
            memset(&fcopts, 0, sizeof(fcopts));

            opts.flags = CAN_ISOTP_DEFAULT_FLAGS;
            opts.flags |= (CAN_ISOTP_EXTEND_ADDR | CAN_ISOTP_RX_EXT_ADDR);            

            if (can_mngr->protocol == CAN_PROTOCOL_ISOTP_TYPE_ARQ) {
                opts.ext_address    = CAN_SOCKET_ADDR_OPT | can_mngr->rx_id;
                opts.rx_ext_address = CAN_SOCKET_ADDR_OPT | can_mngr->tx_id;
            }
            else {
                opts.ext_address    = can_mngr->rx_id;
                opts.rx_ext_address = can_mngr->tx_id;
            }

            fcopts.bs = 0;
            fcopts.wftmax = 200;
            fcopts.stmin = 1;
            setsockopt(can_mngr->socket, SOL_CAN_ISOTP, CAN_ISOTP_OPTS, &opts, sizeof(opts));
            setsockopt(can_mngr->socket, SOL_CAN_ISOTP, CAN_ISOTP_RECV_FC, &fcopts, sizeof(fcopts));
        }

        int ret = bind(can_mngr->socket, (struct sockaddr*)&addr, sizeof(addr));        
        if (ret < 0) {
            SLOGE("Unable to open CAN socket Id %d", can_mngr->id);
            continue;
        }
    }

    FUNC_EXIT();
}

void modbus_service_intf_deinit(void)
{
    
}

can_mngr_t* modbus_get_mngr(can_mngr_id_t mngr_id)
{
    assert(mngr_id < CAN_MNGR_MAX);
    return &can_mngr_tbl[mngr_id];
}

void modbus_service_intf_tx_control(bool is_enable)
{
    is_enable_tx = is_enable;
}

int32_t modbus_service_intf_recv(can_mngr_id_t mngr_id, uint8_t* data, uint16_t dlc)
{
    assert((mngr_id < CAN_MNGR_MAX) && data);

    can_mngr_t* can_mngr = &can_mngr_tbl[mngr_id];

    pthread_mutex_lock(&can_mutex[can_mngr->channel]);
    int read_cnt = read(can_mngr->socket, data, dlc);
    pthread_mutex_unlock(&can_mutex[can_mngr->channel]);

    return read_cnt;
}

int32_t modbus_service_intf_send(can_mngr_id_t mngr_id, uint8_t* data, uint16_t dlc)
{
    assert((mngr_id < CAN_MNGR_MAX) && data);

    can_mngr_t* can_mngr = &can_mngr_tbl[mngr_id];
    
    pthread_mutex_lock(&can_mutex[can_mngr->channel]);
    int write_cnt = write(can_mngr->socket, data, dlc);
    pthread_mutex_unlock(&can_mutex[can_mngr->channel]);

    return write_cnt;
}
