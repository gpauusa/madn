#include <stdio.h>
#include <assert.h>
#include "madn_pkt_struct.h"


int main()
{
    printf("Testing madn_pkt_struct...\n");

    //test create APIs
    MADN_PKT_LEECH* leech = NULL;
    MADN_PKT_DATA* data = NULL;
    MADN_PKT_BEACON* beacon = NULL;

    leech = create_leech_pkt();
    assert(leech != NULL);
    data = create_data_pkt();
    assert(data != NULL);
    beacon = create_beacon_pkt();
    assert(beacon != NULL);

    //check that packet types are set by the constructors
    assert(get_packet_type((MADN_PTR) leech) == LEECH);
    assert(get_packet_type((MADN_PTR) data) == DATA);
    assert(get_packet_type((MADN_PTR) beacon) == BEACON);

    //check that entries are set to zero
    assert(get_packet_entries((MADN_PTR) leech) == 0);
    assert(get_packet_entries((MADN_PTR) data) == 0);
    assert(get_packet_entries((MADN_PTR) beacon) == 0);

    //set entries and check that they're set correctly
    set_packet_entries((MADN_PTR) leech, 234);
    set_packet_entries((MADN_PTR) data, 67);
    set_packet_entries((MADN_PTR) beacon, 91);
    assert(get_packet_entries((MADN_PTR) leech) == 234);
    assert(get_packet_entries((MADN_PTR) data) == 67);
    assert(get_packet_entries((MADN_PTR) beacon) == 91);

    free_req_pkt(&leech);
    free_data_pkt(&data);
    free_beacon_pkt(&beacon);
    assert(leech == NULL);
    assert(data ==  NULL);
    assert(beacon == NULL);


    printf("Success!\n");
    
    return 0;
}
