#include "madn_coder.h"
#include "madn_cache.h"
#include "madn_register_local.h"
#include <assert.h>

uint8_t decode_packet(MADN_INSTANCE* env, MADN_PKT_DATA* pkt)
{
    //true if packet is needed (not in cache, false if it is)
    uint8_t ret = add_coded_packet_cache(env, pkt);


    if (check_decoded_cache(env, pkt->id) == 1)
    {
        //call the callback function
        madn_execute_callback(env, pkt->id);
    }


    return ret;
}

void encode_packet(MADN_INSTANCE* env, MADN_PKT_DATA* pkt)
{
    if (check_decoded_cache(env, pkt->id) == 1)
    {
        //TODO:fill in rubbish and send back
    }
    else
    {
        //FAIL
        //should never reach here
        assert(0);
    }

}
