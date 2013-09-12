#ifndef __MADN_CACHE_H__
#define __MADN_CACHE_H__

#include "madn_main_struct.h"
#include "madn_pkt_struct.h"

typedef struct {

        MADN_DATAID id;
        uint8_t aducount;
        uint8_t adumax;
        void* data; //shall be a GQueue* for coded packets, char** after decoding

} CACHE_ENTRY;


uint8_t create_cache(MADN_INSTANCE* env);
uint8_t destroy_cache(MADN_INSTANCE* env);

uint8_t add_coded_packet_cache(MADN_INSTANCE* env, MADN_PKT_DATA* pkt); //decoder uses
uint8_t check_decoded_cache(MADN_INSTANCE* env, MADN_DATAID id); //handle_leech uses
char** get_decoded_data(MADN_INSTANCE* env, MADN_DATAID id, uint8_t* size); //encoder uses

CACHE_ENTRY* create_cache_entry();
void destroy_cache_entry(CACHE_ENTRY** entry);

CACHE_ENTRY* check_entry_cache(MADN_INSTANCE *env, MADN_DATAID id);
CACHE_ENTRY* get_entry_cache(MADN_INSTANCE *env, MADN_DATAID id);
uint8_t remove_entry_cache(MADN_INSTANCE *env, MADN_DATAID id);



//LEGACY CODE

//typedef unsigned int (*madn_lookup_cache)(const int c_id, const int stripe_id, const int pkt_id);
//typedef unsigned int (*madn_store_cache)(const int c_id, const int stripe_id, const int pkt_id, const int len, const char* data);
//typedef unsigned int (*madn_read_cache)(const int c_id, const int stripe_id, const int pkt_id, const int len, const char* data, const char* buf);

//    madn_lookup_cache lookup;
//    madn_read_cache read;
//    madn_store_cache store;
//    void *cache_data;
//    void *cache_extra;

#endif
