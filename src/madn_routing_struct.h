/*
 * madn_route_struct.h
 *
 *  Created on: Feb 5, 2011
 *  Author: fabio
 */

#ifndef MADN_ROUTE_STRUCT_H_
#define MADN_ROUTE_STRUCT_H_

#include <time.h>
#include <assert.h>
#include "madn_main_struct.h"
#include "madn_pkt_struct.h"


/** CONSTRUCTOR AND DESTRUCTORS */

uint8_t create_request_table(MADN_INSTANCE *env);       //rt.c
uint8_t destroy_request_table(MADN_INSTANCE *env);      //rt.c

uint8_t create_snb_table(MADN_INSTANCE *env);           //nb.c
uint8_t destroy_snb_table(MADN_INSTANCE *env);          //nb.c

uint8_t create_service_queue(MADN_INSTANCE *env);       //queues.c
uint8_t destroy_service_queue(MADN_INSTANCE *env);      //queues.c

uint8_t create_forward_queue(MADN_INSTANCE *env);       //queues.c
uint8_t destroy_forward_queue(MADN_INSTANCE *env);      //queues.c

uint8_t create_leech_queue(MADN_INSTANCE *env);         //queues.c
uint8_t destroy_leech_queue(MADN_INSTANCE *env);        //queues.c


/** REQUEST TABLE */

REQUEST_ENTRY* get_entry_rt(MADN_INSTANCE *env, MADN_DATAID id);    //rt.c
uint8_t remove_entry_rt(MADN_INSTANCE *env, MADN_DATAID id);        //rt.c
REQUEST_ENTRY* check_entry_rt(MADN_INSTANCE *env, MADN_DATAID id);    //rt.c

/** NEIGHBORHOOD */

NODE_ENTRY* get_entry_nb(MADN_INSTANCE *env, NODEID id);            //nb.c
uint8_t remove_entry_nb(MADN_INSTANCE *env, NODEID id);             //nb.c
void generate_self_bloom(MADN_INSTANCE *env);


/** QUEUES */
//Service Queue
MADN_DATAID* add_entry_sq(MADN_INSTANCE *env, MADN_DATAID id);  //queues.c
MADN_DATAID* remove_entry_sq(MADN_INSTANCE *env);               //queues.c
uint8_t is_empty_sq(MADN_INSTANCE *env);                        //queues.c

//Forwarding Queue
MADN_PKT_DATA* add_entry_fq(MADN_INSTANCE *env, MADN_PKT_DATA* data);  //queues.c
MADN_PKT_DATA* remove_entry_fq(MADN_INSTANCE *env);               //queues.c
uint8_t is_empty_fq(MADN_INSTANCE *env);                        //queues.c

//Leech Queue
MADN_DATA_REQUEST* add_entry_lq(MADN_INSTANCE *env, MADN_DATA_REQUEST req); //queues.c
MADN_DATA_REQUEST* remove_entry_lq(MADN_INSTANCE *env);               //queues.c
uint8_t is_empty_lq(MADN_INSTANCE *env);                        //queues.c

#endif /* MADN_ROUTE_STRUCT_H_ */
