#include "madn_cache.h"
#include "madn_settings.h"
#include <glib.h>
#include <stdlib.h>
#include <assert.h>

#define CACHE(var) ((GHashTable*)var->mod_cache)


uint8_t create_cache(MADN_INSTANCE* env)
{
    env->mod_cache = g_hash_table_new(g_int64_hash, g_int64_equal);
    return 0;

}

uint8_t destroy_cache(MADN_INSTANCE* env)
{
    //TODO: make sure table is empty, else free all keys first
    g_hash_table_destroy(CACHE(env));
    return 0;

}

gboolean g_pieces_equal(gconstpointer p1, gconstpointer p2)
{
    if (memcmp(p1, p2, PIECES_DIM) == 0)
    {
        return TRUE;
    }
    return FALSE;
}

uint8_t add_coded_packet_cache(MADN_INSTANCE* env, MADN_PKT_DATA* pkt)
{
    uint8_t ret = 0;

    //first see if entry is already there
    CACHE_ENTRY* entry = get_entry_cache(env, pkt->id);
    if (entry->aducount == 0)
    {
#ifdef TEST
        printf("DBG: Created cache entry\n");
#endif
        //new entry
        entry->adumax = env->globals->ADU_MAX;
        ret = 1;

        //create the hash table
        entry->data = g_hash_table_new(g_int_hash, g_int_equal);
    }
    //assert(entry->adumax == pkt->pieces[PIECES_DIM-1]);    

    //if already decoded, ignore
    if (entry->aducount == entry->adumax)
    {
#ifdef TEST
        printf("DBG: Already decoded\n");
#endif
        return ret;
    }

    //collapse pieces
    uint32_t hash = collapse_pieces(&pkt->pieces); 

    //check if it is already in the HT
    char* data = g_hash_table_lookup((GHashTable*) entry->data, &hash);
    if (data != NULL)
    {
        //already present
#ifdef TEST
        printf("DBG: Already present in hash table\n");
#endif
        //return ret; TODO: remove comment when encoder actually starts sending meaningful data
    }

    //if not, add to hash table
    //duplicate pieces and data as they are going to be destroyed
    //data = malloc(sizeof(char) * MAX_DATA);
    //PIECES* pieces = create_pieces();
    //memcpy(data, &pkt->data, MAX_DATA);
    //copy_pieces(pieces, &pkt->pieces);
    
    //g_hash_table_insert((GHashTable*) entry->data, &hash, &data);
    ret = 1;
#ifdef TEST
    printf("DBG: Inserted data into hash table\n");
#endif

    entry->aducount++;
 
    return ret;

}

uint8_t check_decoded_cache(MADN_INSTANCE* env, MADN_DATAID id)
{

    //TODO:remove later
    //return 0;

    CACHE_ENTRY* entry = check_entry_cache(env, id);
    if (entry == NULL) return 0;

    printf("Packets recd: %d\n", entry->aducount);
    if (entry->aducount == entry->adumax)
    {
        return 1;
    }

    return 0; //false
}

char** get_decoded_data(MADN_INSTANCE* env, MADN_DATAID id, uint8_t* size)
{
    CACHE_ENTRY* entry = check_entry_cache(env, id);
    if (entry == NULL)
    {
        *size = 0;
        return NULL;
    }

    *size = entry->adumax;
    return entry->data;
}

CACHE_ENTRY* create_cache_entry()
{
    CACHE_ENTRY* entry = calloc(1, sizeof(CACHE_ENTRY));
    assert(entry != NULL);
    return entry;
}

void free_cache_entry(CACHE_ENTRY** entry)
{
    /*
    if ((*entry)->aducount == (*entry)->adumax)
    {
        //it is decoded, so a char** array
        int i;
        char** tmp = (char**) (*entry)->data;
        for (i = 0; i < (*entry)->adumax; i++)
        {
            free(*(tmp + i));
        }
        free(tmp);
    }
    else
    {
    */
        //it is still coded

        //TODO: make sure table is empty, else free all keys first
        g_hash_table_destroy((GHashTable*)(*entry)->data);
        (*entry)->data = NULL;
    //}


    free(*entry);
    *entry = NULL;
}

CACHE_ENTRY* add_entry_cache(MADN_INSTANCE *env, MADN_DATAID id)
{
    CACHE_ENTRY *entry = create_cache_entry();
    copy_dataid(&entry->id, &id);
    g_hash_table_insert(CACHE(env), &entry->id, entry);
    return entry;
}

CACHE_ENTRY* check_entry_cache(MADN_INSTANCE *env, MADN_DATAID id)
{
    CACHE_ENTRY *entry = g_hash_table_lookup(CACHE(env), &id);
    return entry;
}

CACHE_ENTRY* get_entry_cache(MADN_INSTANCE *env, MADN_DATAID id)
{
    CACHE_ENTRY *entry = g_hash_table_lookup(CACHE(env), &id);
    if (entry == NULL) entry = add_entry_cache(env, id);
    return entry;
}

uint8_t remove_entry_cache(MADN_INSTANCE *env, MADN_DATAID id)
{
    //first retrieve the stored data
    CACHE_ENTRY *entry = g_hash_table_lookup(CACHE(env), &id);

    //now remove from hash table
    g_hash_table_remove(CACHE(env), &id);

    //free fetched data
    if (entry != NULL) free_cache_entry(&entry);
    assert(entry == NULL);

    return 0;
}

