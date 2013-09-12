#include <stdio.h>
#include <assert.h>
#include "madn_main_struct.h"
#include "madn_pkt_struct.h"
#include "madn_routing.h"
#include "madn_routing_struct.h"
#include "madn_cache.h"
#include "madn_coder.h"

int main()
{

//uint8_t decode_packet(MADN_INSTANCE* env, MADN_PKT_DATA* pkt)
//void encode_packet(MADN_INSTANCE* env, MADN_PKT_DATA* pkt)

    printf("Testing cache...\n");

    MADN_INSTANCE* instance = create_madn_instance();
    assert(instance != NULL);

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
    assert(instance->mod_cache != NULL);

    //Create a few data packets
    MADN_PKT_DATA* data1 = create_data_pkt();
    set_packet_node((MADN_PTR) data1, 17);
    strncpy((char*) &data1->bloom, "abcdefghijklmno", 16);
    data1->id.chunk = 456;
    data1->id.stripe = 870;
    strncpy((char*) &data1->pieces, "abcdefghijklmnopqrstuvwxyzABCDE", 32);
    strcpy((char*) &data1->data, "Mary had a little lamb.");

    assert(decode_packet(instance, data1) == 1);
    assert(decode_packet(instance, data1) == 0);

    MADN_PKT_DATA* data2 = create_data_pkt();
    set_packet_node((MADN_PTR) data2, 19);
    strncpy((char*) &data2->bloom, "ABCDEFGHIJKLMNO", 16);
    data2->id.chunk = 456;
    data2->id.stripe = 870;
    strncpy((char*) &data2->pieces, "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDE", 32);
    strcpy((char*) &data2->data, "A quick brown fox jumped over the lazy dog.");

    assert(decode_packet(instance, data2) == 1);

    MADN_PKT_DATA* data3 = create_data_pkt();
    set_packet_node((MADN_PTR) data3, 21);
    strncpy((char*) &data3->bloom, "pqrstuvwxyzabcd", 16);
    data3->id.chunk = 267;
    data3->id.stripe = 9238;
    strncpy((char*) &data3->pieces, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcde", 32);
    strcpy((char*) &data3->data, "Twinkle twinkle little star.");

    assert(decode_packet(instance, data3) == 1);

    MADN_PKT_DATA* data4 = create_data_pkt();
    set_packet_node((MADN_PTR) data4, 21);
    strncpy((char*) &data4->bloom, "PQRSTUVWXYZABCD", 16);
    data4->id.chunk = 456;
    data4->id.stripe = 850;
    strncpy((char*) &data4->pieces, "abcdefghijklmnopqrstuvwxyzabcde", 32);
    strcpy((char*) &data4->data, "Baa baa black sheep, have you any wool?");

    assert(decode_packet(instance, data4) == 1);

    free_data_pkt(&data1);
    free_data_pkt(&data2);
    free_data_pkt(&data3);
    free_data_pkt(&data4);

    printf("Success!\n");
    return 0;

}
