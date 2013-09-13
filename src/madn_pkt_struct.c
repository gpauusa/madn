#include "madn_pkt_struct.h"
#include <stdlib.h>
#include <stdio.h>

uint8_t get_packet_type(MADN_PTR p)
{
    DATA_PACKET_HEADER *header = (DATA_PACKET_HEADER*) p;
    return header->packet_type;
}

void set_packet_type(MADN_PTR p, uint8_t packet_type)
{
    DATA_PACKET_HEADER *header = (DATA_PACKET_HEADER*) p;
    header->packet_type = packet_type;
}

uint8_t get_packet_entries(MADN_PTR p)
{
    DATA_PACKET_HEADER *header = (DATA_PACKET_HEADER*) p;
    return header->entries;
}

void set_packet_entries(MADN_PTR p, uint8_t entries)
{
    DATA_PACKET_HEADER *header = (DATA_PACKET_HEADER*) p;
    header->entries = entries;
}

NODEID get_packet_node(MADN_PTR p)
{
    DATA_PACKET_HEADER *header = (DATA_PACKET_HEADER*) p;
    return header->id;
}

void set_packet_node(MADN_PTR p, NODEID id)
{
    DATA_PACKET_HEADER *header = (DATA_PACKET_HEADER*) p;
    header->id = id;
}

MADN_PKT_LEECH *create_leech_pkt()
{
        MADN_PKT_LEECH* tmp = (MADN_PKT_LEECH*) calloc(1, sizeof(MADN_PKT_LEECH));
        tmp->header.packet_type = LEECH;
        tmp->header.entries = 0;
        return tmp;
}

MADN_PKT_DATA *create_data_pkt()
{
        MADN_PKT_DATA* tmp = (MADN_PKT_DATA*) calloc(1, sizeof(MADN_PKT_DATA));
        tmp->header.packet_type = DATA;
        tmp->header.entries = 0;
        return tmp;
}

MADN_PKT_BEACON *create_beacon_pkt()
{
        MADN_PKT_BEACON* tmp = (MADN_PKT_BEACON*) calloc(1,sizeof(MADN_PKT_BEACON));
        tmp->header.packet_type = BEACON;
        tmp->header.entries = 0;
        return tmp;
}

MADN_DATA_REQUEST *create_data_request()
{
    MADN_DATA_REQUEST* tmp = (MADN_DATA_REQUEST*) calloc(1,sizeof(MADN_DATA_REQUEST));
    return tmp;
}

void copy_data_pkt(MADN_PKT_DATA* dst, MADN_PKT_DATA* src)
{
    memcpy(dst, src, sizeof(MADN_PKT_DATA));
}

void copy_data_request(MADN_DATA_REQUEST* dst, MADN_DATA_REQUEST* src)
{
    memcpy(dst, src, sizeof(MADN_DATA_REQUEST));
}

void free_req_pkt(MADN_PKT_LEECH **pkt)
{
	free(*pkt);
    *pkt = NULL;
}

void free_data_pkt(MADN_PKT_DATA **pkt)
{
	free(*pkt);
    *pkt = NULL;
}

void free_beacon_pkt(MADN_PKT_BEACON **pkt)
{
	free(*pkt);
    *pkt = NULL;
}

void free_data_request(MADN_DATA_REQUEST **pkt)
{
    free(*pkt);
    *pkt = NULL;
}

void fprint_header(FILE *f, char *pkt)
{
     fprint_field_32(f, pkt, HEADER_DIM);
}
void fprint_beacon(FILE *f, char *pkt)
{
    fprint_header(f, pkt);
    pkt += HEADER_DIM;
    fprint_bloom(f, pkt);
}

void fprint_req(FILE *f, char *pkt)
{
    fprint_dataid(f, pkt);
    pkt += DATAID_DIM;
    fprint_field_8(f, pkt, 2);
}

void fprint_leech(FILE *f, char *pkt)
{
    int i;
    int len = get_packet_entries(pkt);

    fprint_header(f, pkt);
    pkt += HEADER_DIM;
    fprint_bloom(f, pkt);
    pkt += MADN_BLOOM_DIM;
    for (i = 0; i < len; i++)
    {
        fprint_req(f, pkt+(i * sizeof(MADN_DATA_REQUEST)));
    }
}

void fprint_data(FILE *f, char *pkt)
{

    int len = get_packet_entries(pkt);

    fprint_header(f, pkt);
    pkt += HEADER_DIM;
    fprint_bloom(f, pkt);
    pkt += MADN_BLOOM_DIM;
    fprint_dataid(f, pkt);
    pkt += DATAID_DIM;
    fprint_field_8(f, pkt, len);

}
