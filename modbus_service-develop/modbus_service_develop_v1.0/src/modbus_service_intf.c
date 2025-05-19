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

#include "logger.h"

#define MODBUS_DEV_NUM                       2
#define MODBUS_DEV_NAME_LEN                  10

#define MODBUS_SOCKET_ADDR_OPT               0x10

#define MODBUS_ADDRESS_EMTER                 0x01
#define MODBUS_ADDRESS_ISOCHA                0x03

static pthread_mutex_t modbus_mutex[MODBUS_DEV_NUM];

static bool is_enable_tx = false;

static modbus_t *ctx = NULL;

void modbus_service_intf_init(void)
{
    FUNC_ENTRY();
    ctx = modbus_new_rtu(DEVICE, BAUD, PARITY, DATA_BIT, STOP_BIT);
    if (ctx == NULL) {
        SLOGE("%s", "Unable to create the libmodbus context");
        return MODBUS_ERR_NOT_INITIALIZED;
    }

    if (modbus_connect(ctx) == -1) {
        SLOGE("%s %s", "Unable to create the libmodbus context", modbus_strerror(errno));
        modbus_free(ctx);
        return MODBUS_ERR_NOT_CONNECTED;
    }

    return MODBUS_ERR_NONE;
    FUNC_EXIT();
}

void modbus_service_intf_deinit(void)
{
    FUNC_ENTRY();
    if (ctx != NULL) {
        modbus_close(ctx);
        modbus_free(ctx);
    }

    return MODBUS_ERR_NONE;
    FUNC_EXIT();
}

modbus_err_t modbus_intf_read_request(int slave_id, uint32_t reg_address, uint32_t nb_regs, uint16_t* read_value){
    FUNC_ENTRY();
    // Set the slave ID
    if (modbus_set_slave(ctx, slave_id) == -1) {
        SLOGE("%s %d %s", "Failed to set slave ID", slave_id, modbus_strerror(errno));
        return MODBUS_ERR_NOT_INITIALIZED;
    }

    // Read the holding register
    if (modbus_read_registers(ctx, reg_address, nb_regs, read_value) == -1) {
        SLOGE("%s %d %s", "Read failed for slave", slave_id, modbus_strerror(errno));
        return MODBUS_ERR_SEND_REQ_FAILED;
    }

    LOGE("%s %d %d", "Read from slave ", slave_id, read_value);
    return MODBUS_ERR_NONE;
    FUNC_EXIT();
}

modbus_err_t modbus_intf_write_request(int slave_id, uint32_t reg_address, uint32_t write_value){
    FUNC_ENTRY();
    // Set the slave ID
    if (modbus_set_slave(ctx, slave_id) == -1) {
        SLOGE("%s %d %s", "Failed to set slave ID", slave_id, modbus_strerror(errno));
        return MODBUS_ERR_NOT_INITIALIZED;
    }

    if (modbus_write_register(ctx, reg_address, write_value) == -1) {
        SLOGE("%s %d %s", "Write failed for slave", slave_id, modbus_strerror(errno));
        return MODBUS_ERR_SEND_REQ_FAILED;
    }

    SLOGE("%s %d %s %d", "Wrote ", write_value, "to slave", slave_id);

    return MODBUS_ERR_NONE;
    FUNC_EXIT();
}