
#define LOG_TAG "can_utils"

#include "can_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <math.h>
#include <byteswap.h>

#include "logger.h"

#include "cancomm_types.h"

extern can_msg_t can_msg_tbl[CAN_MSG_MAX];
extern can_sig_t can_sig_tbl[CAN_SIG_MAX];

static pthread_mutex_t sig_data_mutex;

static void can_message_control(can_msg_id_t msg_id, bool is_active)
{
    if (msg_id < CAN_MSG_MAX) {
        can_msg_t* can_msg = &can_msg_tbl[msg_id];
        if (can_msg->type == CAN_TX_MSG) {
            can_msg->active = is_active;
        }
    }
    else {
        for (uint8_t i = 0; i < CAN_MSG_MAX; i++) {
            can_msg_t* can_msg = &can_msg_tbl[i];
            if (can_msg->type == CAN_TX_MSG) {
                can_msg->active = is_active;
            }
        }
    }
}

void can_set_signal(can_sig_id_t sig_id, uint64_t data)
{
    assert(sig_id < CAN_SIG_MAX);

    can_sig_t* can_sig        = &can_sig_tbl[sig_id];
    uint8_t    can_sig_offset = can_sig->start + can_sig->len;
    
    uint64_t sig_mask = ~((1ULL << can_sig->start) - 1);
    if (can_sig_offset < 64) {
        sig_mask &= ((1ULL << can_sig_offset) - 1);
    }

    data <<= can_sig->start;
    data &= sig_mask;

    pthread_mutex_lock(&sig_data_mutex);
    uint64_t pool_data;
    memmove(&pool_data, can_msg_tbl[can_sig->msg_id].frame.data, sizeof(pool_data));
    pthread_mutex_unlock(&sig_data_mutex);

    pool_data &= ~sig_mask;
    pool_data |= data;

    memmove(can_msg_tbl[can_sig->msg_id].frame.data, &pool_data, sizeof(pool_data));
}

void can_get_grp_signal(can_sig_id_t* grp_sig_id, uint8_t grp_len, uint32_t* data)
{
    assert(data);
    assert(grp_sig_id);

    *data = 0;
    if (grp_len <= 32)
    {        
        for (int i = 0; i < grp_len; i++)
        {
            uint32_t tmp = 0;
            can_get_signal(grp_sig_id[i], &tmp);
            *data |= (tmp << i);
        }
    }
}


void can_get_signal(can_sig_id_t sig_id, void* data)
{
    static uint8_t sig_size_tbl[] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8};

    can_sig_t* can_sig        = &can_sig_tbl[sig_id];
    uint8_t    can_sig_offset = can_sig->start + can_sig->len;

    uint64_t sig_mask = ~((1ULL << can_sig->start) - 1);
    if (can_sig_offset < 64) {
        sig_mask &= ((1ULL << can_sig_offset) - 1);
    }

    pthread_mutex_lock(&sig_data_mutex);
    uint64_t pool_data;
    memmove(&pool_data, can_msg_tbl[can_sig->msg_id].frame.data, sizeof(pool_data));
    pthread_mutex_unlock(&sig_data_mutex);
    
    uint64_t sig_val = (pool_data & sig_mask) >> can_sig->start;     
    memcpy(data, &sig_val, sig_size_tbl[can_sig->len]);
}

bool can_chk_signal(can_sig_id_t sig_id, void* buffer)
{
    assert(buffer);
    
    can_sig_t* can_sig        = &can_sig_tbl[sig_id];
    uint8_t    can_sig_offset = can_sig->start + can_sig->len;

    uint64_t sig_mask = ~((1ULL << (can_sig->start)) - 1);
    if (can_sig_offset < 64) {
        sig_mask &= ((1ULL << can_sig_offset) - 1);
    }

    uint64_t new_val = sig_mask & *(uint64_t*)buffer;
    uint64_t cur_val;
    pthread_mutex_lock(&sig_data_mutex);
    memmove(&cur_val, can_msg_tbl[can_sig->msg_id].frame.data, sizeof(cur_val));
    pthread_mutex_unlock(&sig_data_mutex);
    cur_val &= sig_mask;
    
    return cur_val != new_val;
}

can_msg_id_t can_get_rx_msg_id(uint32_t msg_id)
{
    for (uint8_t i = 0; i < CAN_MSG_MAX; i ++) {
        can_msg_t* can_msg = &can_msg_tbl[i];
        if ((can_msg->type == CAN_RX_MSG) && (can_msg->frame.can_id == msg_id)) {
            return (can_msg_id_t)i;
        }
    }

    return CAN_MSG_MAX;
}

can_msg_t* can_get_msg(can_msg_id_t msg_id)
{
    assert(msg_id < CAN_MSG_MAX);
    return &can_msg_tbl[msg_id];
}

void can_update_msg_data(can_msg_id_t msg_id, uint8_t* frame_data)
{
    assert((msg_id < CAN_MSG_MAX) && frame_data);
    can_msg_t* can_msg = &can_msg_tbl[msg_id];
    memcpy(can_msg->frame.data, frame_data, can_msg->frame.can_dlc);
}

void can_activate_tx_msg(can_msg_id_t msg_id)
{
    can_message_control(msg_id, true);
}

void can_deactivate_tx_msg(can_msg_id_t msg_id)
{
    can_message_control(msg_id, false);
}

bool can_is_tx_msg_actived(can_msg_id_t msg_id)
{
    assert(msg_id < CAN_MSG_MAX);
    return can_msg_tbl[msg_id].active;
}

void can_dump(can_transfer_t dir, uint8_t type, uint8_t channel, uint32_t id, uint8_t* data, uint8_t dlc)
{
    assert(data);
    
    printf("%s: type %d, ch %d, id %X, data", (dir == CAN_TX_MSG) ? "Send" : "Recv", type, channel, id);
    for (uint8_t i = 0; i < dlc; i++) {
        printf(" %02X", data[i]);
    }
    printf("\n");
}

float rec_present_output_convert(uint32_t present)
{
    int8_t Exponent1;
    float Exponent, Mantissa = 0;
    Exponent1 = ((present >> 23) & 0x000000ff) - 127;
    Exponent = pow(2, Exponent1);
    if (((present >> 23) & 0x000000ff) > 0)
    {
        Mantissa = 1;
    }
    if ((present & 0b00000000010000000000000000000000) > 0)
    {
        Mantissa = Mantissa + 0.5;
    }
    if ((present & 0b00000000001000000000000000000000) > 0)
    {
        Mantissa = Mantissa + 0.5 / 2;
    }
    if ((present & 0b00000000000100000000000000000000) > 0)
    {
        Mantissa = Mantissa + 0.5 / 4;
    }
    if ((present & 0b00000000000010000000000000000000) > 0)
    {
        Mantissa = Mantissa + 0.5 / 8;
    }
    if ((present & 0b00000000000001000000000000000000) > 0)
    {
        Mantissa = Mantissa + 0.5 / 16;
    }
    if ((present & 0b00000000000000100000000000000000) > 0)
    {
        Mantissa = Mantissa + 0.5 / 32;
    }
    if ((present & 0b00000000000000010000000000000000) > 0)
    {
        Mantissa = Mantissa + 0.5 / 64;
    }
    if ((present & 0b00000000000000001000000000000000) > 0)
    {
        Mantissa = Mantissa + 0.5 / 128;
    }
    if ((present & 0b00000000000000000100000000000000) > 0)
    {
        Mantissa = Mantissa + 0.5 / 256;
    }
    if ((present & 0b00000000000000000010000000000000) > 0)
    {
        Mantissa = Mantissa + 0.5 / 512;
    }
    if ((present & 0b00000000000000000001000000000000) > 0)
    {
        Mantissa = Mantissa + 0.5 / 1024;
    }
    if ((present & 0b00000000000000000000100000000000) > 0)
    {
        Mantissa = Mantissa + 0.5 / 2048;
    }
    if ((present & 0b00000000000000000000010000000000) > 0)
    {
        Mantissa = Mantissa + 0.5 / 4096;
    }
    if ((present & 0b00000000000000000000001000000000) > 0)
    {
        Mantissa = Mantissa + 0.5 / 8192;
    }
    if ((present & 0b00000000000000000000000100000000) > 0)
    {
        Mantissa = Mantissa + 0.5 / 16384;
    }
    if ((present & 0b00000000000000000000000010000000) > 0)
    {
        Mantissa = Mantissa + 0.5 / 32768;
    }
    if ((present & 0b00000000000000000000000001000000) > 0)
    {
        Mantissa = Mantissa + 0.5 / 65536;
    }
    if ((present & 0b00000000000000000000000000100000) > 0)
    {
        Mantissa = Mantissa + 0.5 / 131072;
    }
    if ((present & 0b00000000000000000000000000010000) > 0)
    {
        Mantissa = Mantissa + 0.5 / 262144;
    }
    if ((present & 0b00000000000000000000000000001000) > 0)
    {
        Mantissa = Mantissa + 0.5 / 524288;
    }
    if ((present & 0b00000000000000000000000000000100) > 0)
    {
        Mantissa = Mantissa + 0.5 / 1048576;
    }
    if ((present & 0b00000000000000000000000000000010) > 0)
    {
        Mantissa = Mantissa + 0.5 / 2097152;
    }
    if ((present & 0b00000000000000000000000000000001) > 0)
    {
        Mantissa = Mantissa + 0.5 / 4194304;
    }
    if (((present >> 31) & 0x00000001) > 0)
    {
        return -1 * Exponent * Mantissa;
    }
    else
    {
        return Exponent * Mantissa;
    }
}
