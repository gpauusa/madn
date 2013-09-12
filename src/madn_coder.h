#ifndef __MADN_CODER_H__
#define __MADN_CODER_H__

#include "madn_main_struct.h"
#include "madn_pkt_struct.h"

uint8_t decode_packet(MADN_INSTANCE* env, MADN_PKT_DATA* pkt);
void encode_packet(MADN_INSTANCE* env, MADN_PKT_DATA* pkt); //pkt has dataid set



#endif
