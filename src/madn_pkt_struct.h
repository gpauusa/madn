#ifndef __MADN_PKT_STRUCT_H__
#define __MADN_PKT_STRUCT_H__

#include "madn_main_struct.h"
#include <stdint.h>
#include <string.h>

#define BEACON 2
#define LEECH 4
#define DATA 8

#define HEADER_DIM 8 //8 byte header
#define MAX_DATA_ID_REQUESTS 160
#define MAX_DATA 1400

typedef struct {
	NODEID id;                                      //4 byte
	uint8_t packet_type;                            //1 byte
	uint8_t entries;                                //1 byte
	uint8_t padding[HEADER_DIM - NODEID_DIM - 2];   //8-2-2 = 2 byte
} DATA_PACKET_HEADER;

typedef struct {
    MADN_DATAID id;                                 //8 bytes
    uint8_t pkts;                                   //1 byte
    uint8_t hops;                                   //1 byte
} MADN_DATA_REQUEST;                                //10 bytes

typedef struct {
        DATA_PACKET_HEADER header;                  //8 bytes
        MADN_BLOOM bloom;                           //16 bytes
        MADN_DATA_REQUEST request[MAX_DATA_ID_REQUESTS]; //10*160 = 1600 bytes
        //MADN_DATA_REQUEST *request; //variable sized leech packets (10-1600 bytes)
} MADN_PKT_LEECH;

typedef struct {
	DATA_PACKET_HEADER header;                      //8 bytes
	MADN_BLOOM bloom;                               //16 bytes
	MADN_DATAID id;                                 //8 bytes
    PIECES pieces;                                  //32 bytes
	unsigned char data[MAX_DATA];                 //1400 bytes
    //unsigned char* data; //variable sized           //1-1400 bytes
} MADN_PKT_DATA;

typedef struct {
    DATA_PACKET_HEADER header;                  //8 bytes
    MADN_BLOOM bloom;                           //16 bytes
} MADN_PKT_BEACON;

uint8_t get_packet_type(MADN_PTR p);
void set_packet_type(MADN_PTR p, uint8_t packet_type);

uint8_t get_packet_entries(MADN_PTR p);
void set_packet_entries(MADN_PTR p, uint8_t entries);

NODEID get_packet_node(MADN_PTR p);
void set_packet_node(MADN_PTR p, NODEID id);

MADN_PKT_LEECH *create_leech_pkt();
MADN_PKT_DATA *create_data_pkt();
MADN_PKT_BEACON *create_beacon_pkt();
MADN_DATA_REQUEST *create_data_request();

void copy_data_pkt(MADN_PKT_DATA* dst, MADN_PKT_DATA* src);
void copy_data_request(MADN_DATA_REQUEST* dst, MADN_DATA_REQUEST* src);

void free_req_pkt(MADN_PKT_LEECH **pkt);
void free_data_pkt(MADN_PKT_DATA **pkt);
void free_beacon_pkt(MADN_PKT_BEACON **pkt);
void free_data_request(MADN_DATA_REQUEST **pkt);

#endif
