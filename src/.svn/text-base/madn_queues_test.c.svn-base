#include <stdio.h>
#include <assert.h>
#include <glib.h>
#include "madn_routing.h"
#include "madn_routing_struct.h"
#include "madn_settings.h"
#include "madn_bloom.h"

void beacon(MADN_INSTANCE *instance)
{
    //Insert some beacon packets, see that SNB is built
    MADN_PKT_BEACON* bcn1 = create_beacon_pkt();
    set_packet_node((MADN_PTR) bcn1, 17);
    strncpy((char*) &bcn1->bloom, "abcdefghijklmno", 16);

    MADN_PKT_BEACON* bcn2 = create_beacon_pkt();
    set_packet_node((MADN_PTR) bcn2, 19);
    strncpy((char*) &bcn2->bloom, "ABCDEFGHIJKLMNO", 16);

    MADN_PKT_BEACON* bcn3 = create_beacon_pkt();
    set_packet_node((MADN_PTR) bcn3, 21);
    strncpy((char*) &bcn3->bloom, "pqrstuvwxyzabcd", 16);

    MADN_PKT_BEACON* bcn4 = create_beacon_pkt();
    set_packet_node((MADN_PTR) bcn4, 21);
    //need a bit of more work for the second beacon
    bloom_add_node(instance->hash, bcn4->bloom, MADN_BLOOM_DIM, instance->globals->NODE_ID);
    //strncpy((char*) &bcn4->bloom, "PQRSTUVWXYZABCD", 16);

    handle_packet((MADN_PTR) bcn1, instance);
    handle_packet((MADN_PTR) bcn2, instance);
    handle_packet((MADN_PTR) bcn3, instance);
    handle_packet((MADN_PTR) bcn4, instance);

    //glib specific part here for testing
    //check size of hash table
    //printf("%d\n", g_hash_table_size((GHashTable*) instance->mod_snb));
    assert(g_hash_table_size((GHashTable*) instance->mod_snb) == 3);
    //check that entries can be retrieved
    NODE_ENTRY* n1 = get_entry_nb(instance, 17);
    assert(n1 != NULL);
    assert(n1->node == 17);
    assert(n1->counter == 1);
    fprint_time(stdout, &n1->lchk);

    NODE_ENTRY* n2 = get_entry_nb(instance, 21);
    assert(n2 != NULL);
    assert(n2->node == 21);
    assert(n2->counter == 2);
    fprint_time(stdout, &n2->lchk);

    NODE_ENTRY* n3 = get_entry_nb(instance, 19);
    assert(n3 != NULL);
    assert(n3->node == 19);
    assert(n3->counter == 1);
    fprint_time(stdout, &n3->lchk);

    nb_thread(instance);

    //remove_entry_nb(instance, 19);
    //remove_entry_nb(instance, 17);
    //remove_entry_nb(instance, 21);

    MADN_BLOOM blm;
    memset(blm, 0, MADN_BLOOM_DIM);
    bloom_add_node(instance->hash, blm, MADN_BLOOM_DIM, 19);
    bloom_add_node(instance->hash, blm, MADN_BLOOM_DIM, 17);
    bloom_add_node(instance->hash, blm, MADN_BLOOM_DIM, 21);

    assert(memcmp(blm, instance->bloom, MADN_BLOOM_DIM) == 0);
    assert(g_hash_table_size((GHashTable*) instance->mod_snb) == 0);
}

void data(MADN_INSTANCE *instance)
{
    //create a few data packets
    MADN_PKT_DATA* data1 = create_data_pkt();
    set_packet_node((MADN_PTR) data1, 17);
    strncpy((char*) &data1->bloom, "abcdefghijklmno", 16);
    data1->id.chunk = 456;
    data1->id.stripe = 870;
    strncpy((char*) &data1->pieces, "abcdefghijklmnopqrstuvwxyzABCDE", 32);
    strcpy((char*) &data1->data, "Mary had a little lamb.");

    MADN_PKT_DATA* data2 = create_data_pkt();
    set_packet_node((MADN_PTR) data2, 19);
    strncpy((char*) &data2->bloom, "ABCDEFGHIJKLMNO", 16);
    data2->id.chunk = 12897;
    data2->id.stripe = 28973;
    strncpy((char*) &data2->pieces, "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDE", 32);
    strcpy((char*) &data2->data, "A quick brown fox jumped over the lazy dog.");

    MADN_PKT_DATA* data3 = create_data_pkt();
    set_packet_node((MADN_PTR) data3, 21);
    strncpy((char*) &data3->bloom, "pqrstuvwxyzabcd", 16);
    data3->id.chunk = 267;
    data3->id.stripe = 9238;
    strncpy((char*) &data3->pieces, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcde", 32);
    strcpy((char*) &data3->data, "Twinkle twinkle little star.");

    MADN_PKT_DATA* data4 = create_data_pkt();
    set_packet_node((MADN_PTR) data4, 21);
    strncpy((char*) &data4->bloom, "PQRSTUVWXYZABCD", 16);
    data4->id.chunk = 456;
    data4->id.stripe = 870;
    strncpy((char*) &data4->pieces, "abcdefghijklmnopqrstuvwxyzabcde", 32);
    strcpy((char*) &data4->data, "Baa baa black sheep, have you any wool?");

    assert(is_empty_fq(instance) != 0);
    handle_packet((MADN_PTR) data1, instance);
    handle_packet((MADN_PTR) data2, instance);
    handle_packet((MADN_PTR) data3, instance);
    handle_packet((MADN_PTR) data4, instance);

    assert(g_queue_get_length((GQueue*) instance->mod_fq) == 4);

    //now retrieve items and check
    data1 = remove_entry_fq(instance);
    assert(get_packet_node((MADN_PTR) data1) == 17);
    assert(strcmp((char*) &data1->bloom, "abcdefghijklmno") == 0);
    assert(data1->id.chunk == 456);
    assert(data1->id.stripe == 870);
    assert(strcmp((char*) &data1->pieces, "abcdefghijklmnopqrstuvwxyzABCDE") == 0);
    assert(strcmp((char*) &data1->data, "Mary had a little lamb.") == 0);

    data2 = remove_entry_fq(instance);
    assert(get_packet_node((MADN_PTR) data2) == 19);
    assert(strcmp((char*) &data2->bloom, "ABCDEFGHIJKLMNO") == 0);
    assert(data2->id.chunk == 12897);
    assert(data2->id.stripe == 28973);
    assert(strcmp((char*) &data2->pieces, "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDE") == 0);
    assert(strcmp((char*) &data2->data, "A quick brown fox jumped over the lazy dog.") == 0);

    data3 = remove_entry_fq(instance);
    assert(get_packet_node((MADN_PTR) data3) == 21);
    assert(strcmp((char*) &data3->bloom, "pqrstuvwxyzabcd") == 0);
    assert(data3->id.chunk == 267);
    assert(data3->id.stripe == 9238);
    assert(strcmp((char*) &data3->pieces, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcde") == 0);
    assert(strcmp((char*) &data3->data, "Twinkle twinkle little star.") == 0);

    data4 = remove_entry_fq(instance);
    assert(get_packet_node((MADN_PTR) data4) == 21);
    assert(strcmp((char*) &data4->bloom, "PQRSTUVWXYZABCD") == 0);
    assert(data4->id.chunk == 456);
    assert(data4->id.stripe == 870);
    assert(strcmp((char*) &data4->pieces, "abcdefghijklmnopqrstuvwxyzabcde") == 0);
    assert(strcmp((char*) &data4->data, "Baa baa black sheep, have you any wool?") == 0);

    assert(g_queue_get_length((GQueue*) instance->mod_fq) == 0);

    assert(is_empty_fq(instance) != 0);

    free_data_pkt(&data1);
    free_data_pkt(&data2);
    free_data_pkt(&data3);
    free_data_pkt(&data4);

    assert(data1 == NULL);
    assert(data2 == NULL);
    assert(data3 == NULL);
    assert(data4 == NULL);

}

void request(MADN_INSTANCE *instance)
{
    MADN_PKT_LEECH* lch1 = create_leech_pkt();
    set_packet_node((MADN_PTR) lch1, 17);
    set_packet_entries((MADN_PTR) lch1, 4);
    strncpy((char*) &lch1->bloom, "abcdefghijklmno", 16);
    int i;
    for (i = 0; i < 4; i++)
    {
        lch1->request[i].id.chunk = i;
        lch1->request[i].id.stripe = i+5;
        lch1->request[i].pkts = 20;
        lch1->request[i].hops = i;
    }

    MADN_PKT_LEECH* lch2 = create_leech_pkt();
    set_packet_node((MADN_PTR) lch2, 19);
    set_packet_entries((MADN_PTR) lch2, 8);
    strncpy((char*) &lch1->bloom, "abcdefghijklmno", 16);
    for (i = 0; i < 8; i++)
    {
        lch2->request[i].id.chunk = i;
        lch2->request[i].id.stripe = i+10;
        lch2->request[i].pkts = 10;
        lch2->request[i].hops = instance->globals->REQ_HOPS;
    }

    handle_packet((MADN_PTR) lch1, instance);
    handle_packet((MADN_PTR) lch2, instance);
    
    assert(g_hash_table_size((GHashTable*) instance->mod_rq) == 12);

    //check that all values stored correctly in req table
    for (i = 0; i < 4; i++)
    {
        MADN_DATAID* id = create_dataid();
        id->chunk = i;
        id->stripe = i+5;
        REQUEST_ENTRY *r = get_entry_rt(instance, *id);
        assert(r->dataid.chunk == i);
        assert(r->dataid.stripe == i+5);
        assert(r->counter == 20);
    }
    
    for (i = 0; i < 8; i++)
    {
        MADN_DATAID* id = create_dataid();
        id->chunk = i;
        id->stripe = i+10;
        REQUEST_ENTRY *r = get_entry_rt(instance, *id);
        assert(r->dataid.chunk == i);
        assert(r->dataid.stripe == i+10);
        assert(r->counter == 10);
    }

    //look at request queue and see that everything is in order
    assert(g_queue_get_length((GQueue*) instance->mod_lq) == 12);
    for (i = 0; i < 4; i++)
    {
        MADN_DATA_REQUEST *req = remove_entry_lq(instance);
        assert(req->id.chunk == i);
        assert(req->id.stripe == i+5);
        assert(req->pkts == 20);
        assert(req->hops == i);
        add_entry_lq(instance, *req);
        free_data_request(&req);
    }

    for (i = 0; i < 8; i++)
    {
        MADN_DATA_REQUEST *req = remove_entry_lq(instance);
        assert(req->id.chunk == i);
        assert(req->id.stripe == i+10);
        assert(req->pkts == 10);
        assert(req->hops == instance->globals->REQ_HOPS);
        add_entry_lq(instance, *req);
        free_data_request(&req);
    }
    assert(g_queue_get_length((GQueue*) instance->mod_lq) == 12);

    leech_thread(instance);

}

int main()
{
    printf("Testing queues...\n");
    
    //checking the int to string function in bloom.h
    char id_str[15];
    memset(id_str, 0, 15);
    nodeidToString(id_str, 0);
    assert(strcmp(id_str, "0") == 0);
    memset(id_str, 0 ,15);
    nodeidToString(id_str, 893674);
    assert(strcmp(id_str, "893674") == 0);
    memset(id_str, 0, 15);
    nodeidToString(id_str, 5);
    assert(strcmp(id_str, "5") == 0);

    MADN_INSTANCE* instance = create_madn_instance();
    assert(instance != NULL);
    read_madn_settings("madn.conf", instance->globals);

    int ret = create_structures(instance);
    assert(ret == 0);
    assert(instance->mod_rq != NULL);
    assert(instance->mod_snb != NULL);
    assert(instance->mod_sq != NULL);
    assert(instance->mod_lq != NULL);
    assert(instance->mod_fq != NULL);
    assert(instance->hash != NULL);
    assert(instance->hash->nfuncs == 1);
    assert(*(instance->hash->funcs) == &sax_hash);
    
    beacon(instance);
    data(instance);
    request(instance);

    printf("Success!\n");
    return 0;
}
