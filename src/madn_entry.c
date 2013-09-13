#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <assert.h>

#include "madn_main_struct.h"
#include "madn_pkt_struct.h"
#include "madn_routing.h"
#include "madn_threading.h"
#include "madn_register_local.h"
#include "madn_socket_helper.h"

//include application handler headers


void testfunc(MADN_DATAID id)
{
    printf("Recd data!!!\n");
}


int main()
{
    //set config parameters

    //create structures
    MADN_INSTANCE* instance = create_madn_instance();
    
    read_madn_settings("madn.conf", instance->globals);
    //printf("%d\n",instance->globals->NODE_NUM);
    print_madn_settings(instance->globals);
    InitSockets(instance);

    //printf("BEACON:%lu\nLEECH:%lu\nDATA:%lu\n", sizeof(MADN_PKT_BEACON), sizeof(MADN_PKT_LEECH), sizeof(MADN_PKT_DATA));
    uint8_t ret = create_structures(instance);
    if (ret != 0)
    {
        printf("Error creating MADN data structures!\n");
    }

    //register applications
    //MADN_DATAID id;
    //id.chunk = 5;
    //id.stripe = 15;
    //id.stripe = 35;

    //madn_register_server(instance, id);
    //printf("%p\n", &testfunc);
    //madn_register_receiver(instance, id, 5, &testfunc);
    //madn_register_receiver(instance, id, 5, &testfunc);

    //assert(g_queue_get_length((GQueue*) instance->mod_lq) == 2);


    //start threading
    create_threads(instance);

    //never reaches here
    return 0;
}
