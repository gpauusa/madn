#include "madn_register_local.h"
#include "madn_settings.h"
#include "madn_cache.h"
#include "madn_coder.h"
#include "madn_routing.h"
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>


//This function inserts dummy data into the cache, fooling it into
//believing that data is being served locally
//This does not really require a callback
uint8_t madn_register_server(MADN_INSTANCE *env, MADN_DATAID id)
{
    srand(time(NULL));
    while(check_decoded_cache(env, id) != 1)
    {
        
        MADN_PKT_DATA* data1 = create_data_pkt();
        set_packet_node((MADN_PTR) data1, env->globals->NODE_ID);
        strncpy((char*) &data1->bloom, "abcdefghijklmno", 16);
        data1->id.chunk = id.chunk;
        data1->id.stripe = id.stripe;
        int j;
        for (j = 0; j < PIECES_DIM/sizeof(int); j++)
        {
            data1->pieces[j] = rand();
        }
        //strncpy((char*) &data1->pieces, "abcdefghijklmnopqrstuvwxyzABCDE", 32);
        strcpy((char*) &data1->data, "Mary had a little lamb.");

        //add_coded_packet_cache(env, data1);
        decode_packet(env, data1);

        free_data_pkt(&data1);
        //assert(ret != 0);
    }

    return 0;
}

//This function adds the callback function to the hashtable maintained for
//received packets callback and sends a single request packet to the
//handle_packet function
//
//This may need to be called repeatedly in case of packet loss, etc.
//// NO MORE: Leech packet creation has been moved to a new thread.
uint8_t madn_register_receiver(MADN_INSTANCE *env, MADN_DATAID id, uint8_t hops, void (*func) (MADN_DATAID))
{
    //register func with local callback hash table
    if (env->mod_local_cb == NULL)
    {
        //create a new hash table
        env->mod_local_cb = g_hash_table_new(g_int64_hash, g_int64_equal);
    }

    if (g_hash_table_lookup((GHashTable*) env->mod_local_cb, &id) == NULL)
    {
        //do a memcopy for id
        MADN_DATAID* dataid = create_dataid();
        copy_dataid(dataid, &id);
        //printf("%d %d %p\n", dataid->chunk, dataid->stripe, func);
        g_hash_table_insert((GHashTable*) env->mod_local_cb, dataid, func);
    }
    else
    {
        //already exists so replace
        g_hash_table_insert((GHashTable*) env->mod_local_cb, &id, func);
    }

    //create request and send it to handle_packet
    MADN_PKT_LEECH* lch = create_leech_pkt();
    set_packet_node((MADN_PTR) lch, env->globals->NODE_ID);
    set_packet_entries((MADN_PTR) lch, 1);
    lch->request[0].id.chunk = id.chunk;
    lch->request[0].id.stripe = id.stripe;
    lch->request[0].hops = hops;
    lch->request[0].pkts = env->globals->ADU_MAX;

    handle_leech(lch, env);

    return 0;
}

void madn_execute_callback(MADN_INSTANCE* env, MADN_DATAID id)
{
    if (env->mod_local_cb == NULL) return;

    void (*func)(MADN_DATAID) = g_hash_table_lookup((GHashTable*) env->mod_local_cb, &id);
 
    //printf("%d %d %p\n", id.chunk, id.stripe, func);

    if (func != NULL)
    {
        (*func) (id);
        //deregister (if required)
        //remove will also work, but key mem will have to be freed first
        g_hash_table_insert((GHashTable*) env->mod_local_cb, &id, NULL);
    }
}

/*
int local_req_thread_helper(void* key, void* val, void* lc)
{
    MADN_DATAID* id = (MADN_DATAID*) key;
    MADN_PKT_LEECH* lch = (MADN_PKT_LEECH*) lc;

    if (val == NULL)
    {
        free_dataid(&id);
        return TRUE;
    }


    int j = get_packet_entries((MADN_PTR) lch);
    lch->request[j].id.chunk = id.chunk;
    lch->request[j].id.stripe = id.stripe;
    //lch->request[j].hops = 4;
    lch->request[j].pkts = env->globals->ADU_MAX;
    set_packet_entries((MADN_PTR) lch, get_packet_entries((MADN_PTR) lch) + 1);
    return FALSE;

}

void local_req_thread(MADN_INSTANCE* env)
{

    if (env->mod_local_cb == NULL) return;

    //create request and send it to handle_packet
    MADN_PKT_LEECH* lch = create_leech_pkt();
    set_packet_node((MADN_PTR) lch, env->globals->NODE_ID);
    set_packet_entries((MADN_PTR) lch, 0);

    g_hash_table_foreach_remove(env->mod_local_cb, local_req_thread_helper, lch);

    int i;
    for (i = 0; i < get_packet_entries; i++)
    {


    }

    handle_leech(lch, env);

}
*/
