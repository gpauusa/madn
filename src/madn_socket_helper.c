#include "madn_socket_helper.h"
#include "madn_routing.h"
#include "madn_settings.h"
#include <assert.h>

//Aditya: all moved to madn_settings.h

//char ethDevice[]="wlan0";
//long int sleepBeaconSend=2000000;//200ms
//long int sleepRequestSend=1000000;//800ms
//long int sleepReceive=10;//10 
//long int sleepRemakeFilter=10000000;//in seconds
//long int sleepOnEmptyQ=0;//default gets set to five times that of smaller interval packet
//unsigned char eth_flags[4] = {0x00,0x00,0x00,0x00};
//unsigned char prot_id[2] = {0x00,0x00};
//unsigned char src_mac[ETH_ALEN] = {0x6a,0xc4,0x06,0x52,0x02,0x7b};
//unsigned char dest_mac[ETH_ALEN] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
//unsigned char broadcast8bitMAC[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

//#define BROADCASTADDRLEN 8

void reader_func(MSRPacket* packet, void* env)
{
    handle_packet((MADN_PTR) packet->buffer, (MADN_INSTANCE*) env);
    free(packet);
}

void InitializeSocketAddrLL(struct sockaddr_ll  *socket_addr_ll, MADN_SETTINGS* globals)
{
    socket_addr_ll->sll_family   = PF_PACKET;
    socket_addr_ll->sll_protocol = htons(ETH_P_ALL);
    socket_addr_ll->sll_ifindex  = if_nametoindex(globals->ethDevice);//2; //index
    socket_addr_ll->sll_hatype   = ARPHRD_ETHER; //hardware identifier - ethernet
    socket_addr_ll->sll_pkttype  = PACKET_OTHERHOST;
    socket_addr_ll->sll_halen    = ETH_ALEN; //address length 
    MSR_CopyAddressArray(socket_addr_ll->sll_addr,globals->broadcast8bitMAC,globals->BROADCASTADDRLEN);   
}

void SetUserDefinedMsgValues(MSRMsgDetails *msgInfo, MADN_SETTINGS* globals)
{
    msgInfo->socket_addr_ll=(struct sockaddr_ll*)malloc(sizeof(struct sockaddr_ll));
    msgInfo->sockaddr_len=(socklen_t*)malloc(sizeof(socklen_t));
    msgInfo->msg=(MSRPacket*)malloc(sizeof(MSRPacket));
    *msgInfo->sockaddr_len=0;
    msgInfo->msgFlags=0;
    msgInfo->addrLength=ETH_ALEN;
    msgInfo->bufferlength=ETH_FRAME_LEN;    
    InitializeSocketAddrLL(msgInfo->socket_addr_ll, globals);
}

void addToOutputQueue(MADN_INSTANCE *env, MADN_PTR* buf, size_t length)
{

    MSRMsgDetails *msgInfo = calloc(1, sizeof(MSRMsgDetails));

    SetUserDefinedMsgValues(msgInfo, env->globals);

    msgInfo->msg=(MSRPacket*)malloc(sizeof(MSRPacket));
    msgInfo->msg->forwardFlag=0;//when 0, it creates, when 1 it forwards
    msgInfo->msg->buffer=buf;
    msgInfo->msg->length=length;
    
    //printf("SENDING Reqs = %d\n", ((MADN_PKT_LEECH*) buf)->header.entries);
    MSR_PushQ(env->si->sendQ, msgInfo, &(env->si->sendQLock));
}


void SetUserDefinedSendValues(MSRSendInfo *si, MADN_SETTINGS* globals)
{
    //Send info object
    MSR_SendInitialize(si);
    
    //Socket details, used in the socket(domain,type,protocol) call.
    si->sock->domain=AF_PACKET;
    si->sock->protocol=ETH_P_ALL;
    si->sock->type=SOCK_RAW;
    //Message details and address details of the message target
    MSR_CopyAddressArray(globals->src_mac,si->src_mac,ETH_ALEN);
    MSR_CopyAddressArray(globals->dest_mac,si->dest_mac,ETH_ALEN);
    strcpy(si->bind->interface,globals->ethDevice);

    InitializeSocketAddrLL(&(si->bind->addr), globals);
    //beacDetails= (MSRMsgDetails*)malloc(sizeof(MSRMsgDetails));
    //reqDetails= (MSRMsgDetails*)malloc(sizeof(MSRMsgDetails));
    //Define beacdetails
    //si->sleepOnEmptyQ=globals->sleepOnEmptyQ;
    si->maxSend=40000;
    
    si->resultcode=MSR_CreateSocket(si->sock);
    assert(si->resultcode == 0);
}

void SetUserDefinedRecvValues(MSRRecvInfo *ri, MADN_SETTINGS* globals)
{
    MSR_RecvInitialize(ri);
    //Socket properties
    ri->sock->domain=PF_PACKET;
    ri->sock->protocol=ETH_P_ALL;
    ri->sock->type=SOCK_RAW;
    //Bind properties
    ri->bind->addr.sll_family = PF_PACKET;
    ri->bind->addr.sll_protocol = htons(ETH_P_ALL);
    ri->bind->addr.sll_pkttype = PACKET_BROADCAST;
    ri->bind->addr.sll_halen=ETH_ALEN;
    ri->bind->addr.sll_hatype=SOCK_RAW;
    //beaconQ=g_queue_new();
    strcpy(ri->bind->interface,globals->ethDevice);
    //Executer function properties
    ri->executer=reader_func;//handle_packet;//Assumed functrion parameter is MSRPacket
    //Message receive properties.
    ri->maxReceive=120;
    ri->msgDetail->bufferlength=ETH_FRAME_LEN;
    //From MAIN
    ri->msgDetail->sleepTime=globals->sleepReceive;

    ri->resultcode=MSR_CreateSocket(ri->sock);
    assert(ri->resultcode == 0);

}

void InitSockets(MADN_INSTANCE *env)
{
    env->ri=(MSRRecvInfo*)malloc(sizeof(MSRRecvInfo));
    env->si=(MSRSendInfo*)malloc(sizeof(MSRSendInfo));
    SetUserDefinedSendValues(env->si, env->globals);
    SetUserDefinedRecvValues(env->ri, env->globals);
    //MSR_CreateInputQueue(env->ri);
    env->ri->recvQ=g_queue_new();
    MSR_StartReceiverThread(env->ri);
    MSR_CreateOutputQueue(env->si);    
}


void receive_thread(MADN_INSTANCE* env)
{

    MSRRecvInfo * ri= env->ri;
    MSRPacket* queuedPacket = NULL;
    while(1)
    {
        pthread_mutex_lock (&(ri->recvQLock));
        if(!g_queue_is_empty(ri->recvQ))
        {
            queuedPacket = g_queue_pop_head(ri->recvQ);
        }
        else
        {
            pthread_mutex_unlock (&(ri->recvQLock));
            break;
        }
        pthread_mutex_unlock (&(ri->recvQLock));
        printf("\nCalling executer method");
        ri->executer(queuedPacket, (void*) env);
     }
}

