#ifndef _MADN_SETTINGS_H
#define _MADN_SETTINGS_H

#include <stdint.h>
#include <linux/if_ether.h>

typedef struct
{
    uint32_t NODE_ID;           //node id
    uint32_t REQ_FWRD_TIMEOUT;  //
    uint32_t REQ_HOPS;          //default num of hops for lch pkt
    uint32_t ADU_MAX;           //num of data pkts reqd for decoding stripe
    uint32_t BROADCASTADDRLEN;  //madn_socket_helper
    long int sleepBeaconSend;   //not used
    long int sleepRequestSend;  //not send
    long int sleepReceive;      //madn_socket_helper
    long int sleepRemakeFilter; //not used
    long int sleepOnEmptyQ;     //madn_socket_helper
    char ethDevice[80];         //madn_socket_helper
    unsigned char eth_flags[4];
    unsigned char prot_id[2];
    unsigned char src_mac[ETH_ALEN];
    unsigned char dest_mac[ETH_ALEN];
    unsigned char broadcast8bitMAC[8];
    unsigned int SNB_THREAD_INTERVAL; //threading 
    unsigned int BCN_THREAD_INTERVAL; //threading 
    uint32_t MAX_DATA_PKT_FORWARD; //max number of data pkts to fwd at one go
    uint32_t MAX_DATA_PKT_SERVE; //max number of data pkts to serve at one go
    uint32_t BCN_REPEAT_THRESHOLD; //num of bcns to be seen before adding to nbf

} MADN_SETTINGS;

MADN_SETTINGS* create_madn_settings();

uint8_t read_madn_settings(char* filename, MADN_SETTINGS* globals);

void print_madn_settings(MADN_SETTINGS* globals);

//#define REQUEST_FWRD_TIMEOUT 10000 //TODO
//#define REQUEST_HOPS 3
//#define ADUMAX 160 //the number of packets reqd for decoding (madn_cache.c)
//#define NODENUM 12 //TODO: find a better way

#endif  /* _MADN_SETTINGS_H */
