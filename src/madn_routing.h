/* 
 * File:   madn_routing.h
 * Author: fabio
 *
 * Created on May 10, 2011, 3:06 PM
 */

#ifndef _MADN_ROUTING_H
#define	_MADN_ROUTING_H

#include <time.h>
#include "madn_main_struct.h"
#include "madn_pkt_struct.h"
#include "madn_bloom.h"

typedef struct {
    MADN_BLOOM tbf;
    MADN_DATAID dataid;
    time_t lmod;
    time_t lfwrd;
    uint8_t l_flag;
    uint8_t server_flag;
    uint8_t counter;
    uint32_t avg_hop;
} REQUEST_ENTRY; //entries for request table

typedef struct {
    NODEID node;
    time_t lchk;
    uint32_t counter;
} NODE_ENTRY; //entries for sensed neighborhood table
    
/** CONSTRUCTORS/DESTRUCTORS TABLE */
    
uint8_t create_structures(MADN_INSTANCE *env);
uint8_t destroy_structures(MADN_INSTANCE *env);

/** PACKET HANDLING */

void handle_packet(MADN_PTR pkt, MADN_INSTANCE *env);
void handle_leech(MADN_PKT_LEECH *pkt, MADN_INSTANCE *env);
void handle_beacon(MADN_PKT_BEACON *pkt, MADN_INSTANCE *env);
void handle_data(MADN_PKT_DATA *pkt, MADN_INSTANCE *env);


/** ACTIVE THREADS */

void service_thread(MADN_INSTANCE *env);
void forward_thread(MADN_INSTANCE *env);
void leech_thread(MADN_INSTANCE *env);
void beacon_thread(MADN_INSTANCE *env);
void nb_thread(MADN_INSTANCE *env);


/** PRINT STRUCTURES */
void fprint_request_entry(FILE *f, REQUEST_ENTRY *pkt);
void fprint_node_entry(FILE *f,  NODE_ENTRY *pkt);

/** UTILS REQUESTS */

REQUEST_ENTRY* create_request_entry();
void free_request_entry(REQUEST_ENTRY **entry);


/** UTILS NODES */

NODE_ENTRY* create_node_entry();
void free_node_entry(NODE_ENTRY **entry);

/** PRINT FUNCTIONS */
void fprint_time(FILE* f, time_t* t);

#endif	/* _MADN_ROUTING_H */
