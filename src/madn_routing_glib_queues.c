#include <glib.h>
#include "madn_main_struct.h"
#include "madn_routing.h"

#define SQ(var) ((GQueue*)var->mod_sq)
#define FQ(var) ((GQueue*)var->mod_fq)
#define LQ(var) ((GQueue*)var->mod_lq)

uint8_t create_service_queue(MADN_INSTANCE *env)
{
    env->mod_sq = g_queue_new();
    return 0;
}

uint8_t destroy_service_queue(MADN_INSTANCE *env)
{
    //TODO: make sure queue is empty, otherwise need to call free on all items
    g_queue_free(SQ(env));
    return 0;
}

uint8_t create_forward_queue(MADN_INSTANCE *env)
{
    env->mod_fq = g_queue_new();
    return 0;
}
uint8_t destroy_forward_queue(MADN_INSTANCE *env)
{
    //TODO: make sure queue is empty, otherwise need to call free on all items
    g_queue_free(FQ(env));
    return 0;
}

uint8_t create_leech_queue(MADN_INSTANCE *env)
{
    env->mod_lq = g_queue_new();
    return 0;
}

uint8_t destroy_leech_queue(MADN_INSTANCE *env)
{
    //TODO: make sure queue is empty, otherwise need to call free on all items
    g_queue_free(LQ(env));
    return 0;
}

/** QUEUES */
MADN_DATAID* add_entry_sq(MADN_INSTANCE *env, MADN_DATAID id)
{
    MADN_DATAID *tmp = create_dataid();
    copy_dataid(tmp, &id);
    //*tmp = id;
    g_queue_push_tail(SQ(env), tmp);
    return tmp;
    
}
MADN_DATAID* remove_entry_sq(MADN_INSTANCE *env)
{
    return (MADN_DATAID*) g_queue_pop_head(SQ(env));
}
uint8_t is_empty_sq(MADN_INSTANCE *env)
{
    return g_queue_is_empty(SQ(env));
}


MADN_PKT_DATA* add_entry_fq(MADN_INSTANCE *env, MADN_PKT_DATA *data)
{
    MADN_PKT_DATA *tmp = create_data_pkt();
    copy_data_pkt(tmp, data);
    //*tmp = id;
    g_queue_push_tail(FQ(env), tmp);
    return tmp;
    
}

MADN_PKT_DATA* remove_entry_fq(MADN_INSTANCE *env)
{
    return (MADN_PKT_DATA*) g_queue_pop_head(FQ(env));
}

uint8_t is_empty_fq(MADN_INSTANCE *env)
{
    return g_queue_is_empty(FQ(env));
}

MADN_DATA_REQUEST* add_entry_lq(MADN_INSTANCE *env, MADN_DATA_REQUEST id)
{
    MADN_DATA_REQUEST *tmp = create_data_request();
    copy_data_request(tmp, &id);
    //*tmp = id;
    g_queue_push_tail(LQ(env), tmp);
    return tmp;

}
MADN_DATA_REQUEST* remove_entry_lq(MADN_INSTANCE *env)
{
    return (MADN_DATA_REQUEST*) g_queue_pop_head(LQ(env));
}

uint8_t is_empty_lq(MADN_INSTANCE *env)
{
    return g_queue_is_empty(LQ(env));
}
