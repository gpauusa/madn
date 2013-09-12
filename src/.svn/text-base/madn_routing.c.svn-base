//#include "madn_bloom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include "madn_core.h"
//#include "madn_main_struct.h"
#include "madn_pkt_struct.h"
#include "madn_routing.h"
#include "madn_routing_struct.h"
//#include "madn_bloom.h"
#include <assert.h>
#include "madn_settings.h"
#include "madn_cache.h"
#include "madn_coder.h"
#include "madn_socket_helper.h"

/** CONSTRUCTORS/DESTRUCTORS TABLE */
    
uint8_t create_structures(MADN_INSTANCE *env)
{
    create_request_table(env);
    create_snb_table(env);
    create_forward_queue(env);
    create_service_queue(env);
    create_leech_queue(env);
    env->hash = bloom_hash_create(1, sax_hash);
    //bloom_hash_create(env->hash, 2, sax_hash, sdbm_hash);
    assert(&env->hash != NULL);
    create_cache(env);
    return 0;
    
}

uint8_t destroy_structures(MADN_INSTANCE *env)
{
    destroy_request_table(env);
    destroy_snb_table(env);
    destroy_forward_queue(env);
    destroy_leech_queue(env);
    destroy_service_queue(env);
    bloom_hash_destroy(&env->hash);
    destroy_cache(env);
    return 0;
    
}

/** PACKET HANDLING */

void handle_leech(MADN_PKT_LEECH *pkt, MADN_INSTANCE *env)
{
    //When a LCH packet is seen, the following is done:
    //Look at header, check NODEID and entries
    //Look at Bloom Filter inside packet
    //Iterate over all the requests.

    //For each iteration, (refer 6.10)
    //1) Create (or update) entry in request queue
    //2) Look at DATAID. If it can be serviced locally (i.e. exists in 
    //   content cache) add request to Service Queue, else, add to
    //   Request Queue
    //3) Set no. of req packets to max (req, entry in req table) in what? (req?)
    //4) If above has been changed, change lmod in the req table entry
    //5) Insert NODEID of requester into the req table entry (as this is a
    //   request from a packet)

    REQUEST_ENTRY *entry;
    uint8_t i;
    

    for (i = 0; i < get_packet_entries((MADN_PTR) pkt); i++)
    {
        entry = get_entry_rt(env, pkt->request[i].id);
        assert(entry != NULL);
        
        if (get_packet_node((MADN_PTR) pkt) == env->globals->NODE_ID)
        {
            //locally requested
            entry->l_flag = 1;
        }
        else
        {
            //enter nodeid of requester into tbf
            bloom_add_node(env->hash, entry->tbf, MADN_BLOOM_DIM, pkt->header.id);
        }

        //check if data is locally available.
        if (check_decoded_cache(env, pkt->request[i].id))
        {
            // if yes, add request to service queue
            add_entry_sq(env, pkt->request[i].id);
        }
        else
        {
            // TODO:if no, check bloom filter inclusion and add to req queue,
            // XXX:ignore if local
            //if(bloom_gossip(env->bloom, pkt->bloom) == 0)
            //{
                //included, so no need to send
            //    continue;
            //}
            //else
            {
                add_entry_lq(env, pkt->request[i]);
            }
        }

        if (entry->counter < pkt->request[i].pkts)
        {
            entry->counter = pkt->request[i].pkts;
            //update lmod
            time(&entry->lmod);
        }
        else
        {
            pkt->request[i].pkts = entry->counter;
        }
    }
    free_req_pkt(&pkt);
}
void handle_beacon(MADN_PKT_BEACON *pkt, MADN_INSTANCE *env)
{
    //Refer 6.8
    //Check if entry in SNB
    //update time
    //increment counter
    NODE_ENTRY *entry = get_entry_nb(env, pkt->header.id);
    assert(entry != NULL);
    if (entry->counter == 0 || //first time
        bloom_check_node(env->hash, pkt->bloom, MADN_BLOOM_DIM, env->globals->NODE_ID)) //inclusion
    {
        entry->counter++;
        time(&entry->lchk);
    }
    free_beacon_pkt(&pkt);
    
}
void handle_data(MADN_PKT_DATA *pkt, MADN_INSTANCE *env)
{
    // send to rateless decoder, which will decode it and store in content cache
    // send to forwarding queue

    // The decoder can store it in the content cache and then decide
    // if a local
    // application requested it.

    // STORE
    // DOES IT HAVE TO BE FORWARDED
    // DOES IT SERVE A REQUEST

    // Send to rateless decoder
    // Get feedback if pkt is needed. If not needed, do not insert in fq
    if (decode_packet(env, pkt) != 0)
    {
        MADN_PKT_DATA* qpkt = add_entry_fq(env, pkt);
        assert(qpkt != NULL);
    }
    free_data_pkt(&pkt);
}


void handle_packet(MADN_PTR pkt, MADN_INSTANCE *env)
{
    uint8_t type = get_packet_type(pkt);
    switch (type)
    {
        case BEACON:
            handle_beacon((MADN_PKT_BEACON*)pkt, env);
            break;
        case LEECH:
            handle_leech((MADN_PKT_LEECH*)pkt, env);
            break;
        case DATA:
            handle_data((MADN_PKT_DATA*)pkt, env);
            break;
        default:
            //should never reach here
            //assert(0);
            break;
            
    }
}

/** ACTIVE THREADS */
//These are only the procedures inside the threads.
//Thread API specific code will call these functions to do the actual processing

void service_thread(MADN_INSTANCE *env)
{
    //printf("s");
    // Refer 6.12
    // Remove entry from service queue
    // Encode a data packet, if same packet already sent, re-encode
    // Put it in the output queue
    // Decrease packet count from the corresponding req table entry
    // If 0, remove entry from req table and also from service queue
    // Push entry back into queue if req table count not 0

    while (!is_empty_sq(env))
    {
        MADN_DATAID* id = remove_entry_sq(env);
        assert(id != NULL);

        REQUEST_ENTRY* entry = check_entry_rt(env, *id);
        if (entry == NULL)
        {
            free_dataid(&id);
            continue;
        }
        
        MADN_PKT_DATA* pkt = create_data_pkt();
        set_packet_node((MADN_PTR) pkt, env->globals->NODE_ID);
        copy_dataid(&pkt->id, id);
        copy_bloom(&pkt->bloom, &env->bloom);

        entry->counter--;
        if (entry->counter == 0)
        {
            remove_entry_rt(env, *id);
        }
        else
        {
            add_entry_sq(env, *id);
            free_dataid(&id);
        }
        time(&entry->lfwrd); //TODO: here?

        //this automatically checks if the encoded packet was already sent
        encode_packet(env, pkt);

        //XXX: Potential race condition, if the decoded packets get evicted from cache
        
        addToOutputQueue(env, (MADN_PTR*) pkt, sizeof(MADN_PKT_DATA));


    }


}

void forward_thread(MADN_INSTANCE *env)
{
    //printf("f\n");
    // Check if entry in req table. If no entry do not forward
    // compare remote nbf with tbf in req table entry, if tbf included in nbf
    // do not forward
    // if packet sent recently, do not forward
    // substitute NBF with local NBF and add packet to output queue
    
    while (!is_empty_fq(env))
    {
        //get entry from fq
        MADN_PKT_DATA* pkt = remove_entry_fq(env);
        assert(pkt != NULL);
    
        //check if entry in rq table
        REQUEST_ENTRY* entry = check_entry_rt(env, pkt->id);
        if (entry == NULL)
        {
            free_data_pkt(&pkt);
            continue;
        }
        
        //check bloom inclusion
        if (bloom_gossip(env->bloom, pkt->bloom) == 0)
        {
            //tbf was included in nbf
            //do not forward
            free_data_pkt(&pkt);
            continue;
        }

        //how do we know if packet sent recently? TODO

        //all tests passed, replace bloom
        copy_bloom(&pkt->bloom, &env->bloom);
        set_packet_node((MADN_PTR) pkt, env->globals->NODE_ID); //TODO: we should do this, right? 
        time(&entry->lfwrd); //TODO: here?

        addToOutputQueue(env, (MADN_PTR*) pkt, sizeof(MADN_PKT_DATA));
    }
}

void leech_thread(MADN_INSTANCE *env)
{
    //printf("l");
    // This thread is woken up every 'x' time units
    // It looks at the req queue
    // for each request in the request queue (max 160?)
    // 0) If no corresponding entry in req table, discard request (just a check?)
    // 1) checks if req already served (look at packets in req table entry)
    // 2) check lfwrd and lmod. if lmod > lfwrd use the request. If not, 
    //    check if lfwrd is recent. If yes, push the req to back of queue. If no,
    //    use it.
    // 3) Put the created LCH packet in the output queue

    //create a new LCH packet
    MADN_PKT_LEECH* lch = create_leech_pkt();
    set_packet_node((MADN_PTR) lch, env->globals->NODE_ID);
    //get bloom
    copy_bloom(&lch->bloom, &env->bloom);
    int i;

    for (i = 0; i < 160; i++)
    {
        if (is_empty_lq(env)) break;
        MADN_DATA_REQUEST* req = remove_entry_lq(env);
        assert(req != NULL);
        REQUEST_ENTRY *entry = check_entry_rt(env, req->id);

        if (entry == NULL)
        {
            //request not needed as no entry in req table
            free_data_request(&req);
            i--;
            continue;
        }

        if (entry->counter == 0)
        {
            remove_entry_rt(env, req->id);
            free_data_request(&req);
        }

        if (entry->lmod < entry->lfwrd)
        {
            time_t t;
            time(&t);
            if ((entry->lfwrd + env->globals->REQ_FWRD_TIMEOUT) < t) //true -> recent
            {
                add_entry_lq(env, *req);
                continue;
            }
        }
        //use entry
        
        int j = get_packet_entries((MADN_PTR) lch);
        copy_data_request(&lch->request[j], req);

        set_packet_entries((MADN_PTR) lch, get_packet_entries((MADN_PTR) lch) + 1);
    }

    //This should hopefully build the leech packet


#ifdef TEST
    //Testing code -> To be commented
    //check leech
    assert(get_packet_node((MADN_PTR) lch) == env->globals->NODE_ID);
    assert(get_packet_entries((MADN_PTR) lch) == 12);
    assert(get_packet_type((MADN_PTR) lch) == LEECH);
    //bloom check
    
    for (i = 0; i < 4; i++)
    {
        assert(lch->request[i].id.chunk == i);
        assert(lch->request[i].id.stripe == i+5);
        assert(lch->request[i].pkts == 20);
        assert(lch->request[i].hops == i);
    }
    for (i = 0; i < 8; i++)
    {
        assert(lch->request[i+4].id.chunk == i);
        assert(lch->request[i+4].id.stripe == i+10);
        assert(lch->request[i+4].pkts == 10);
        assert(lch->request[i+4].hops == env->globals->REQ_HOPS);
    }
#endif


    //check that there is atleast one request in the packet
    if (get_packet_entries((MADN_PTR) lch) == 0)
    {
        //kill packet
        free_req_pkt(&lch);
    }
    else
    {
        addToOutputQueue(env, (MADN_PTR*) lch, sizeof(MADN_PKT_LEECH));
        
    }


}

void beacon_thread(MADN_INSTANCE *env)
{
    MADN_PKT_BEACON* bcn = create_beacon_pkt();
    set_packet_node((MADN_PTR) bcn, env->globals->NODE_ID);
    copy_bloom(&bcn->bloom, &env->bloom);
    addToOutputQueue(env, (MADN_PTR*) bcn, sizeof(MADN_PKT_BEACON));
}

void nb_thread(MADN_INSTANCE *env)
{
    //printf("n\n");
    // This thread is woken up every 'x' time units
    // Refer 6.8
    // clear NBF
    // add all entries of SNB table into NBF, and delete entry from SNB table

    bloom_reset(env->bloom, MADN_BLOOM_DIM);
    generate_self_bloom(env);

}

/** PRINT STRUCTURES */
void fprint_time(FILE *f, time_t* t)
{
    fprintf(f, "%s ", ctime(t));
}

void fprint_request_entry(FILE *f, REQUEST_ENTRY *entry)
{
    fprint_bloom(f, (MADN_PTR) &entry->tbf);
    fprint_dataid(f, (MADN_PTR) &entry->dataid);
    fprint_time(f, &entry->lmod);
    fprint_time(f, &entry->lfwrd);
    fprint_field_8(f, (MADN_PTR) &entry->l_flag, 1);
    fprint_field_8(f, (MADN_PTR) &entry->server_flag, 1);
    fprint_field_32(f, (MADN_PTR) &entry->counter, 4);
    fprint_field_32(f, (MADN_PTR) &entry->avg_hop, 4);
}

void fprint_node_entry(FILE *f, NODE_ENTRY *entry)
{

    fprint_nodeid(f, (MADN_PTR) &(entry->node));
    fprint_time(f, &entry->lchk);
    fprint_field_32(f, (MADN_PTR) &entry->counter, 4);
}

REQUEST_ENTRY* create_request_entry()
{
    return (REQUEST_ENTRY*) calloc(1, sizeof(REQUEST_ENTRY));
}

NODE_ENTRY* create_node_entry()
{
    return (NODE_ENTRY*) calloc(1, sizeof(NODE_ENTRY));
}

void free_request_entry(REQUEST_ENTRY **entry)
{
    free(*entry);
    *entry = NULL;
}

void free_node_entry(NODE_ENTRY **entry)
{
    free(*entry);
    *entry = NULL;
}
