 /*
 * File:   madn_route_struct.c
 * Author: fabio
 *
 * Created on February 27, 2011, 8:09 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>     
//#include <time.h>
#include <glib.h>
//#include "madn_core.h"
//#include "madn_settings.h"
#include "madn_main_struct.h"
#include "madn_routing.h"
#include "madn_routing_struct.h"
//#include "madn_bloom.h"

#define SNB(var) ((GHashTable*)var->mod_snb)

uint8_t create_snb_table(MADN_INSTANCE *env)
{
    env->mod_snb = g_hash_table_new(g_int_hash, g_int_equal);
    return 0;
}

uint8_t destroy_snb_table(MADN_INSTANCE *env)
{
    //TODO: make sure table is empty, else free all keys first
    g_hash_table_destroy(SNB(env));
    return 0;
}

NODE_ENTRY* add_entry_snb(MADN_INSTANCE *env, NODEID id)
{
    NODE_ENTRY *req = create_node_entry();
    req->node = id;
    g_hash_table_insert(SNB(env), &req->node, req);
    return req;
}

NODE_ENTRY* get_entry_nb(MADN_INSTANCE *env, NODEID id)
{
    NODE_ENTRY *req = g_hash_table_lookup(SNB(env), &id);
    if (req == NULL) req = add_entry_snb(env, id);
    return req;
    
}

uint8_t remove_entry_nb(MADN_INSTANCE *env, NODEID id)
{
    //first retrieve the stored data
    NODE_ENTRY *req = g_hash_table_lookup(SNB(env), &id);

    //now remove from hash table
    g_hash_table_remove(SNB(env), &id);

    //free fetched data
    if (req != NULL) free_node_entry(&req);
    assert(req == NULL);

    return 0;
}

//int add_node_nb_bloom(NODEID* key, NODE_ENTRY* entry, char* bloom)
int add_node_nb_bloom(void* key, void* val, void* en)
{
    //add entry to bloom filter
    NODEID* id = (NODEID*) key;
    NODE_ENTRY* entry = (NODE_ENTRY*) val;
    MADN_INSTANCE* env = (MADN_INSTANCE*) en;

    //TODO: lchk check???
    //TODO: times check???
    bloom_add_node(env->hash, env->bloom, MADN_BLOOM_DIM, *id);
 
    free_node_entry(&entry);

    //asks glib to remove the entry
    return TRUE;
}

void generate_self_bloom(MADN_INSTANCE *env)
{
    //generate bloom magic!
    g_hash_table_foreach_remove(SNB(env), add_node_nb_bloom, env);
}
