/* 
 * File:   MSRstructs.h (MADN SENDER RECEIVER STRUCTURES)
 * Author: root
 *
 * Created on November 18, 2011, 6:44 PM
 */





#ifndef MSRSTRUCTS_H
#define	MSRSTRUCTS_H

//#ifdef	__cplusplus
//extern "C" {
//#endif


//#ifdef	__cplusplus/
//}
//#endif

#include <glib.h>
#include <pthread.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
//#include <linux/if_arp.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include<netdb.h>
//Added by aditya
#include <sys/types.h>
//#include <linux/if.h>
#include <unistd.h>
#include <net/if.h>
#include <net/if_arp.h>
//#include "madn_main_struct.h"


#define MSR_FAIL -1
#define MSR_PASS 0
#define MSR_LOCKFREE 1
#define MSR_LOCKBUSY 0
#define MSR_TRUE 1
#define MSR_FALSE 0

typedef struct
{
    int domain;
    int type;
    int protocol;
    int socketConn;
}MSRSocket;
typedef struct 
{
    void                *buffer;
    int                 length;
    int                 forwardFlag;
}MSRPacket;
typedef struct
{
    struct sockaddr_ll  *socket_addr_ll;
    socklen_t           *sockaddr_len;
    int                 bufferlength;
    
    int                 addrLength;
    int                 msgFlags;
    long int            sleepTime;
    MSRPacket           *msg;         
    
}MSRMsgDetails;

typedef struct
{
    char                *interface;
    struct sockaddr_ll  addr;
}MSRBind;
typedef struct
{
    int                 resultcode;
    int                 endSend;
    //long int            sleepOnEmptyQ;
    pthread_mutex_t     sendQLock;
    int                 maxSend;
    MSRBind             *bind;
    pthread_t           sendThreadID;
    GQueue              *sendQ;
    MSRSocket           *sock;
    unsigned char       src_mac[6];
    unsigned char       dest_mac[6];
}MSRSendInfo;

typedef struct
{
    int                 resultcode;
    int                 endReceive;
    pthread_mutex_t     recvQLock;
    int                 maxReceive;
    MSRBind             *bind;
    pthread_t           recvThreadID;
    pthread_t           execThreadID;
    GQueue              *recvQ;
    MSRMsgDetails       *msgDetail;
    MSRSocket           *sock;
    MSRPacket           *msg;
    void (*executer)(MSRPacket *packet, void* env);

}MSRRecvInfo;



//COMMON
extern int  MSR_CreateSocket(MSRSocket* s);                    //Creates Socket, returns if it passed or failed

/*----------------------RECEIVER----------------------*/
//Functions needed by User
extern void MSR_RecvInitialize(MSRRecvInfo *ri);               //Sets default values initially
extern void MSR_CreateInputQueue(MSRRecvInfo *ri);             //Creates input queue and threads to function 

//End threads. Frees pointers and returns resultcode.
extern int  MSR_StopReceive(MSRRecvInfo *ri);                   //Stops all receive processes and threads gracefully
extern int  MSR_KillReceive(MSRRecvInfo *ri);                   //Kills all threads in receive using SIGKILL

//Main Functions to manipulate receiver
extern void MSR_StartReceiverThread(MSRRecvInfo *ri);            //Starts reader thread
extern void MSR_CreateReceiver(void *recvInfo);               //Creates receiver 
extern void MSR_EndReceiveThread(MSRRecvInfo *ri);             //Ends receive thread


//Main Functions to manipulate executer
extern void MSR_StartExecuterThread(MSRRecvInfo *ri);          //Starts executer thread
extern void MSR_CreateExecuter(void *recvInfo);               //Creates function 
extern void MSR_EndExecuterThread(MSRRecvInfo *ri);            //Ends function thread

//Utility functions to receive a message
extern void MSR_BindReceiver(MSRRecvInfo *ri);                 //Binds receiver to address
extern void MSR_ReceivePacket(MSRRecvInfo * ri, int count);    //Receives packet.
extern void MSR_FreeRecvPointers(MSRRecvInfo *ri);                 //Frees pointers


/*----------------------SENDER----------------------*/
extern void MSR_SendInitialize(MSRSendInfo *si);               //Initializes default values and allocates memory
extern void MSR_CreateOutputQueue(MSRSendInfo *si);            //Creates output queue and starts threads
extern int  MSR_StopSend(MSRSendInfo *si);                     //Stops all send  processes and threads gracefully
extern int  MSR_KillSend(MSRSendInfo *si);                     //Kills all threads in send using SIGKILL
extern void MSR_EndSendThread(MSRSendInfo *si);                 //Ends receive thread

extern void MSR_StartSenderThread(MSRSendInfo *si);            //Starts thread to create function that sends msgs
extern void MSR_CreateSender(void *sendInfo);                 //Function to read packets from sendQ and send

//Utility functions
extern void MSR_CopyAddressArray(unsigned char source[],unsigned char target[],int len);       //Copies array from source to target
extern void MSR_FreeSendPointers(MSRSendInfo *si);                                             //Frees send pointers
extern void MSR_PushQ(GQueue *q, MSRMsgDetails  *msgInfo, pthread_mutex_t *lock);
#endif	/* MSRSTRUCTS_H */
