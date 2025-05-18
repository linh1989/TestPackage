
#ifndef _CAN_UTILS_H_
#define _CAN_UTILS_H_

#include "can_message.h"

void can_set_signal(can_sig_id_t sig_id, uint64_t data);
void can_get_signal(can_sig_id_t sig_id, void* data);
bool can_chk_signal(can_sig_id_t sig_id, void* buffer);
void can_get_grp_signal(can_sig_id_t* grp_sig_id, uint8_t grp_len, uint32_t* data);

can_msg_id_t can_get_rx_msg_id(uint32_t msg_id);
can_msg_t* can_get_msg(can_msg_id_t msg_id);
void can_update_msg_data(can_msg_id_t msg_id, uint8_t* frame_data);

void can_dump(can_transfer_t dir, uint8_t type, uint8_t channel, uint32_t id, uint8_t* data, uint8_t dlc);

void can_activate_tx_msg  (can_msg_id_t msg_id);
void can_deactivate_tx_msg(can_msg_id_t msg_id);
bool can_is_tx_msg_actived(can_msg_id_t msg_id);

float    rec_present_output_convert(uint32_t present);
uint32_t rec_request_output_convert(uint16_t request);

#endif // _CAN_UTILS_H_
