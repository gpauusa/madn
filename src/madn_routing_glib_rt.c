 /*
 * File:   madn_request_table.c
 * Author: fabio
 *
 * Created on February 27, 2011, 8:09 PM
 */

//#include "madn_bloom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <glib.h>
//#include "madn_settings.h"
#include "madn_main_struct.h"
#include "madn_routing.h"
#include "madn_routing_struct.h"

#define RQTABLE(var) ((GHashTable*)(var->mod_rq))

uint8_t create_request_table(MADN_INSTANCE *env)
{
    env->mod_rq = g_hash_table_new(g_int64_hash, g_int64_equal);
    return 0;
}

uint8_t destroy_request_table(MADN_INSTANCE *env)
{
    //TODO: make sure table is empty, else free all keys first
    g_hash_table_destroy(RQTABLE(env));
    return 0;
}

REQUEST_ENTRY* add_entry_rt(MADN_INSTANCE *env, MADN_DATAID id)
{
     REQUEST_ENTRY *req = create_request_entry();
     //req->dataid = id;
     copy_dataid(&req->dataid, &id);
     g_hash_table_insert(RQTABLE(env), &req->dataid, req);
     return req;
    
}

REQUEST_ENTRY* get_entry_rt(MADN_INSTANCE *env, MADN_DATAID id)
{
    REQUEST_ENTRY *req = g_hash_table_lookup(RQTABLE(env), &id);
    if (req == NULL) req = add_entry_rt(env, id);
    return req;
    
}

REQUEST_ENTRY* check_entry_rt(MADN_INSTANCE *env, MADN_DATAID id)
{
    REQUEST_ENTRY *req = g_hash_table_lookup(RQTABLE(env), &id);
    return req;
}

uint8_t remove_entry_rt(MADN_INSTANCE *env, MADN_DATAID id)
{
    //first retrieve the stored data
    REQUEST_ENTRY *req = g_hash_table_lookup(RQTABLE(env), &id);

    //now remove from hash table
    g_hash_table_remove(RQTABLE(env), &id);

    //free fetched data
    if (req != NULL) free_request_entry(&req);
    assert(req == NULL);

    return 0;
}
