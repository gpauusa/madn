#ifndef __MADN_REGISTER_LOCAL_H__
#define __MADN_REGISTER_LOCAL_H__

#include "madn_main_struct.h"
#include "madn_main_struct.h"
#include "madn_pkt_struct.h"

uint8_t madn_register_server(MADN_INSTANCE *env, MADN_DATAID id);

uint8_t madn_register_receiver(MADN_INSTANCE *env, MADN_DATAID id, uint8_t hops, void (*func) (MADN_DATAID));

void madn_execute_callback(MADN_INSTANCE* env, MADN_DATAID id);

#endif
