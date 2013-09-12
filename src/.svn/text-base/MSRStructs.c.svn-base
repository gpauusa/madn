/* 
 * File:   MSRStructs.c
 * Author: Inchara
 *
 * Created on November 18, 2011, 6:42 PM
 */


#include "MSRStructs.h"



/*-----------MAIN FILES BEING ACCESSED BY OUTSIDE PROGRAMS ------------*/


int MSR_CreateSocket(MSRSocket* s)
{
    int result=MSR_PASS;
    printf("\nCreating Socket Connection.. ");
    s->socketConn= socket(s->domain, s->type, htons(s->protocol));
    if (s->socketConn == -1)
    {
        printf("Can't Create socket! "); 
        result=MSR_FAIL;
    }
    return result;
}
void MSR_CreateInputQueue(MSRRecvInfo *ri)
{
    ri->recvQ=g_queue_new();
    MSR_StartReceiverThread(ri);
    MSR_StartExecuterThread(ri);
    
}
void MSR_CreateOutputQueue(MSRSendInfo *si)
{
    si->sendQ=g_queue_new();
    MSR_StartSenderThread(si);
    //MSR_StartPacketAdderThread(si);
}
int MSR_KillReceive(MSRRecvInfo *ri)
{
    int returnValue;
    printf("\nKilling Receive threads");
    pthread_kill(ri->recvThreadID,SIGKILL);
    pthread_kill(ri->execThreadID,SIGKILL);
    g_queue_free(ri->recvQ);
    returnValue=ri->resultcode;
    MSR_FreeRecvPointers(ri);
    return(returnValue);
}
int MSR_StopReceive(MSRRecvInfo *ri)
{
    int returnVal;
    MSR_EndReceiveThread(ri);
    ri->endReceive=MSR_TRUE;
    MSR_EndExecuterThread(ri);    
    g_queue_free(ri->recvQ);
    returnVal=ri->resultcode;
    MSR_FreeRecvPointers(ri);
    return returnVal;
}
int MSR_KillSend(MSRSendInfo *si)
{
    int returnValue;
    printf("\nKilling Send threads");
    pthread_kill(si->sendThreadID,SIGKILL);
    //pthread_kill(si->packetAdderThreadID,SIGKILL);
    g_queue_free(si->sendQ);
    returnValue=si->resultcode;
    MSR_FreeSendPointers(si);
    return(returnValue);
}
int MSR_StopSend(MSRSendInfo *si)
{
    int returnVal;
    MSR_EndSendThread(si);
    si->endSend=MSR_TRUE;
    //MSR_EndPacketAdderThread(si);    
    g_queue_free(si->sendQ);
    returnVal=si->resultcode;
    MSR_FreeSendPointers(si);
    return returnVal;
}
/*------MSR RECV Files------*/
void MSR_RecvInitialize(MSRRecvInfo *ri)
{
    printf("\nAllocating memory to structures");
    pthread_mutex_init(&(ri->recvQLock), NULL);
    ri->sock=(MSRSocket*)malloc(sizeof(MSRSocket));
    ri->msgDetail= (MSRMsgDetails*)malloc(sizeof(MSRMsgDetails));
    ri->bind=(MSRBind*)malloc(sizeof(MSRBind));
    ri->bind->interface=(char*)malloc(sizeof(char)*7);
    //ri->recvQLock=(int*)malloc(sizeof(int));
    ri->msg=NULL;
    
    ri->resultcode=MSR_PASS;
   // *(ri->recvQLock)=MSR_LOCKFREE;
    ri->msgDetail->socket_addr_ll=NULL;
    ri->msgDetail->sockaddr_len=NULL;
    ri->msgDetail->msgFlags=0;
    ri->endReceive=MSR_FALSE;
    
    
    
}
void MSR_FreeRecvPointers(MSRRecvInfo *ri)
{
    printf("\nFreeing Receive Pointers");
    free(ri->sock);
    free(ri->msgDetail);
    free(ri->bind->interface);
    free(ri->bind);
    
    pthread_mutex_destroy(&(ri->recvQLock));
    //free(ri->recvQLock);
    free(ri->msg);
    
    free(ri);
}


void MSR_StartReceiverThread(MSRRecvInfo *ri)
{
    printf("\nStarting Receiver Thread..");
    if(pthread_create(&(ri->recvThreadID),NULL,(void*)&MSR_CreateReceiver,(void *)ri)==-1)
    {
        printf("Receive Thread Create Failed");
        ri->resultcode=MSR_FAIL;
    }
}
void MSR_StartExecuterThread(MSRRecvInfo *ri)
{
    //One thread that takes messages from the queue and does something with it.
    printf("\nStarting Function Thread");
    if(pthread_create(&(ri->execThreadID),NULL,(void*)&MSR_CreateExecuter,(void *)ri)==-1)
    {
        printf("Function Thread Create Failed");
        ri->resultcode=MSR_FAIL;
    }
}

void MSR_EndReceiveThread(MSRRecvInfo *ri)
{
    if((pthread_join(ri->recvThreadID, NULL))==-1)
    {
        printf("Receive Thread Join Failed");
        ri->resultcode=MSR_FAIL;
    }
    else ri->endReceive=1;
}
void MSR_EndExecuterThread(MSRRecvInfo *ri)
{
    if((pthread_join(ri->execThreadID,NULL))==-1)
    {
        printf("Function Thread Join Failed");
        ri->resultcode=MSR_FAIL;
    }
}
void MSR_CreateExecuter(void *recvInfo)
{
    MSRRecvInfo * ri=(MSRRecvInfo*)recvInfo;
    //MSRPacket* queuedPacket = NULL;//=(MSRMsgDetails*)malloc(sizeof(MSRMsgDetails));
    int messageRead=0;
    while(1)
    {
        messageRead=0;
        if(ri->endReceive==MSR_TRUE) break;
        usleep(rand() % 10 + 1);
        pthread_mutex_lock (&(ri->recvQLock));
        if(!g_queue_is_empty(ri->recvQ))
        {
            //queuedPacket = g_queue_pop_head(ri->recvQ);
            messageRead=1;
            
        }
        pthread_mutex_unlock (&(ri->recvQLock));
        if(messageRead)
        {  
            printf("\nCalling executer method");
            //ri->executer(ri->msg) ; Aditya
        }
     }
    pthread_exit(0);
}
void MSR_BindReceiver(MSRRecvInfo *ri)
{
    printf("\nBinding Receive.. ");
    ri->bind->addr.sll_ifindex = if_nametoindex(ri->bind->interface);
    if((bind(ri->sock->socketConn, (struct sockaddr *)&ri->bind->addr, sizeof(ri->bind->addr)))== -1)
    {
        printf("Error Binding");
        ri->resultcode=MSR_FAIL;
    }
    
}
void MSR_CreateReceiver(void *recvInfo)
{
    MSRRecvInfo * ri=(MSRRecvInfo*)recvInfo;
    int count=0;
    usleep(ri->msgDetail->sleepTime*3);
    
    MSR_BindReceiver(ri);
    
    while(count<ri->maxReceive)
    {

        MSR_ReceivePacket(ri,count);
        if(ri->resultcode==MSR_FAIL) continue;
        if(ri->msg!=NULL){
            pthread_mutex_lock (&(ri->recvQLock));
            g_queue_push_tail(ri->recvQ,ri->msg);//Push received packet into receive queue
            pthread_mutex_unlock (&(ri->recvQLock));
            //free(ri->msg);
            //count++;
            usleep(rand() % ri->msgDetail->sleepTime + 1);
        }
    }
    pthread_exit(NULL);
}
void MSR_ReceivePacket(MSRRecvInfo * ri, int count)
{
    char add[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
    MSRPacket* msg = (MSRPacket*)malloc(sizeof(MSRPacket));
    msg->buffer=(void*)malloc(ri->msgDetail->bufferlength);
    msg->length=ri->msgDetail->bufferlength;    
    //int flag=0;//ri->msgDetail->msgFlags;
    ri->resultcode=MSR_PASS;
    int len = recvfrom(ri->sock->socketConn, 
            msg->buffer,
            msg->length,
            ri->msgDetail->msgFlags, 
            (struct sockaddr*)ri->msgDetail->socket_addr_ll, 
            ri->msgDetail->sockaddr_len);
        if (len == -1)
        {
            perror("Error");
            ri->resultcode=MSR_FAIL;
        }
        else 
        {
            if (memcmp(add, msg->buffer, ETH_ALEN)==0)
            {
                 printf("\n Received Broadcast Message number %d, Length : %d ", count+1,len);
            } 
            else
            {
                printf("\nDiscarded Non Broadcast message");
                ri->resultcode=MSR_FAIL;
            }
        }
    ri->msg=msg;
    len=0;
           
}
/*------------------------MSR SEND FILES -------------------*/

void MSR_SendInitialize(MSRSendInfo *si)
{
    pthread_mutex_init(&(si->sendQLock), NULL);
    printf("\nAllocating memory to Send structures");
    si->sock=(MSRSocket*)malloc(sizeof(MSRSocket));
    si->bind=(MSRBind*)malloc(sizeof(MSRBind));
    si->bind->interface=(char*)malloc(sizeof(char)*7);
    //si->dest_mac=(unsigned char*) calloc (len, sizeof(unsigned char));
    //si->src_mac = (unsigned char*) calloc (len, sizeof(unsigned char));
    si->resultcode=MSR_PASS;
    
    si->endSend=FALSE;
    
}
void MSR_FreeSendPointers(MSRSendInfo *si)
{
    printf("\nFreeing Send pointers");
    free(si->sock);
    //free(si->msgDetail);
    //free(si->msg);
    free(si->bind->interface);
    free(si->bind);
    pthread_mutex_destroy(&(si->sendQLock));
    free(si);
}

void MSR_StartSenderThread(MSRSendInfo *si)
{
    printf("\nStarting Sender Thread..");
    if(pthread_create(&(si->sendThreadID),NULL,(void*)&MSR_CreateSender,(void *)si)==-1)
    {
        printf("Send Thread Create Failed");
        si->resultcode=MSR_FAIL;
    }
}

void MSR_EndSendThread(MSRSendInfo *si)
{
    if((pthread_join(si->sendThreadID, NULL))==-1)
    {
        printf("Receive Thread Join Failed");
        si->resultcode=MSR_FAIL;
    }
    else si->endSend=1;
}

void MSR_BindSender(MSRSendInfo *si)
{
    printf("\nBinding Send..");
    if((bind(si->sock->socketConn, (struct sockaddr *)&si->bind->addr, sizeof(si->bind->addr)))== -1)
    {
        perror("Error Binding");
        si->resultcode=MSR_FAIL;
    }
    
}
void MSR_PushQ(GQueue *q, MSRMsgDetails  *msgInfo, pthread_mutex_t *lock)
{
    
    usleep((rand() % 10 + 1));//microseconds. creates number randomly from 0 to 100, sleps max 1 milisecond.
    pthread_mutex_lock (lock);
                g_queue_push_tail(q,msgInfo);
    pthread_mutex_unlock (lock);
    
}
void MSR_CreateSender(void *sendInfo)
{
    MSRSendInfo* si=(MSRSendInfo*)sendInfo;
    int count=0;
    //long int sleepOnEmptyQ=si->sleepOnEmptyQ;
    MSRPacket *msgPacketFromQueue=NULL;
    MSRMsgDetails *msgInfo=NULL;
    int Qempty=0;
    MSR_BindSender(si);
    while(count<si->maxSend)
    {
        usleep((rand() % 10 + 1));//microseconds. creates number randomly from 0 to 100, sleps max 1 milisecond.
        pthread_mutex_lock (&(si->sendQLock));
                Qempty=0;
                if(!g_queue_is_empty(si->sendQ)) 
                    msgInfo=g_queue_pop_head(si->sendQ);
                else  Qempty=1;
        pthread_mutex_unlock (&(si->sendQLock));
        //usleep(msgInfo->sleepTime);//request time sleep. its the smaller of the two(beacon n req)
        if (!Qempty){ 
            msgPacketFromQueue=msgInfo->msg;
            if(sendto(si->sock->socketConn, 
                    msgPacketFromQueue->buffer, 
                    msgPacketFromQueue->length, 
                    msgInfo->msgFlags,
                    (struct sockaddr*)(msgInfo->socket_addr_ll), 
                    sizeof(struct sockaddr_ll))==-1)
            {
                perror("Error!");
                printf("\nCan't Send Packet");
                si->resultcode=MSR_FAIL;
            }
            else printf("\nSent Message %d",count+1);
            count++;
        }
        //sleep(si->msgDetail);
    }
    
        pthread_exit(NULL);
    
}   

void MSR_CopyAddressArray(unsigned char sourceStr[],unsigned char targetStr[],int len)
{
    
    while(--len>=0)
    {
        targetStr[len]=sourceStr[len];
    }
    
}
