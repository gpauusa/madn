#include "madn_settings.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

MADN_SETTINGS* create_madn_settings()
{
    MADN_SETTINGS* ret = calloc(1, sizeof(MADN_SETTINGS));
    assert (ret != NULL);
    return ret;
}

uint8_t read_madn_settings(char* filename, MADN_SETTINGS* globals)
{
    FILE* f = fopen(filename, "r");
    if (f == NULL)
    {
        printf("Couldn't open configuration file\n");
        return 1;
    }

    while (!feof(f))
    {
        char l[80];
        if (fgets(l, 80, f) == NULL)
        {
            fclose(f);
            return 1;
        }

        //tokenize by = sign
        char* token = strtok(l, "=\n");
        if (token != NULL)
        {
            if (strcmp(token, "NODE_ID") == 0)
            {
                token = strtok(NULL, "=\n");
                globals->NODE_ID = atoi(token);
            }
            else if(strcmp(token, "REQ_FWRD_TIMEOUT") == 0)
            {
                token = strtok(NULL, "=\n");
                globals->REQ_FWRD_TIMEOUT = atoi(token);
            }
            else if(strcmp(token, "REQ_HOPS") == 0)
            {
                token = strtok(NULL, "=\n");
                globals->REQ_HOPS = atoi(token);
            }
            else if(strcmp(token, "ADU_MAX") == 0)
            {
                token = strtok(NULL, "=\n");
                globals->ADU_MAX = atoi(token);
            }
            else if(strcmp(token, "BROADCASTADDRLEN") == 0)
            {
                token = strtok(NULL, "=\n");
                globals->BROADCASTADDRLEN = atoi(token);
            }
            else if(strcmp(token, "sleepBeaconSend") == 0)
            {
                token = strtok(NULL, "=\n");
                globals->sleepBeaconSend = atoi(token);
            }
            else if(strcmp(token, "sleepRequestSend") == 0)
            {
                token = strtok(NULL, "=\n");
                globals->sleepRequestSend = atoi(token);
            }
            else if(strcmp(token, "sleepReceive") == 0)
            {
                token = strtok(NULL, "=\n");
                globals->sleepReceive = atoi(token);
            }
            else if(strcmp(token, "sleepRemakeFilter") == 0)
            {
                token = strtok(NULL, "=\n");
                globals->sleepRemakeFilter = atoi(token);
            }
            else if(strcmp(token, "sleepOnEmptyQ") == 0)
            {
                token = strtok(NULL, "=\n");
                globals->sleepOnEmptyQ = atoi(token);
            }
            else if(strcmp(token, "ethDevice") == 0)
            {
                token = strtok(NULL, "=\n");
                strcpy(globals->ethDevice, token);
            }
            else if(strcmp(token, "eth_flags") == 0)
            {
                token = strtok(NULL, "=\n");
                int i;
                char* t = strtok(token, ":");
                for (i = 0; i < 4; i++)
                {
                    sscanf(t, "%X", (unsigned int*) &globals->eth_flags[i]);
                    t = strtok(NULL, ":");
                }
            }
            else if(strcmp(token, "prot_id") == 0)
            {
                token = strtok(NULL, "=\n");
                int i;
                char* t = strtok(token, ":");
                for (i = 0; i < 2; i++)
                {
                    sscanf(t, "%X", (unsigned int*) &globals->prot_id[i]);
                    t = strtok(NULL, ":");
                }
            }
            else if(strcmp(token, "src_mac") == 0)
            {
                token = strtok(NULL, "=\n");
                int i;
                char* t = strtok(token, ":");
                for (i = 0; i < ETH_ALEN; i++)
                {
                    sscanf(t, "%X", (unsigned int*) &globals->src_mac[i]);
                    t = strtok(NULL, ":");
                }
            }
            else if(strcmp(token, "dest_mac") == 0)
            {
                token = strtok(NULL, "=\n");
                int i;
                char* t = strtok(token, ":");
                for (i = 0; i < ETH_ALEN; i++)
                {
                    sscanf(t, "%X", (unsigned int*) &globals->dest_mac[i]);
                    t = strtok(NULL, ":");
                }
            }
            else if(strcmp(token, "broadcast8bitMAC") == 0)
            {
                token = strtok(NULL, "=\n");
                int i;
                char* t = strtok(token, ":");
                for (i = 0; i < 8; i++)
                {
                    sscanf(t, "%X", (unsigned int*) &globals->broadcast8bitMAC[i]);
                    t = strtok(NULL, ":");
                }
            }
            else if(strcmp(token, "SNB_THREAD_INTERVAL") == 0)
            {
                token = strtok(NULL, "=\n");
                globals->SNB_THREAD_INTERVAL = atoi(token);
            }
            else if(strcmp(token, "BCN_THREAD_INTERVAL") == 0)
            {
                token = strtok(NULL, "=\n");
                globals->BCN_THREAD_INTERVAL = atoi(token);
            }

        }

    }

    fclose(f);

	return 0;
}

void print_madn_settings(MADN_SETTINGS* globals)
{
    printf("Printing configuration:\n");
    printf("NODE_ID:\t\t%d\n", globals->NODE_ID);
    printf("REQ_FWRD_TIMEOUT:\t%d\n", globals->REQ_FWRD_TIMEOUT);
    printf("REQ_HOPS:\t\t%d\n", globals->REQ_HOPS);
    printf("ADU_MAX:\t\t%d\n", globals->ADU_MAX);
    printf("BROADCASTADDRLEN:\t%d\n", globals->BROADCASTADDRLEN);
    printf("sleepBeaconSend:\t%ld\n", globals->sleepBeaconSend);
    printf("sleepRequestSend:\t%ld\n", globals->sleepRequestSend);
    printf("sleepReceive:\t\t%ld\n", globals->sleepReceive);
    printf("sleepRemakeFilter:\t%ld\n", globals->sleepRemakeFilter);
    printf("sleepOnEmptyQ:\t\t%ld\n", globals->sleepOnEmptyQ);
    printf("ethDevice:\t\t%s\n", globals->ethDevice);
    printf("eth_flags:\t\t%.2X:%.2X:%.2X:%.2X\n", globals->eth_flags[0], globals->eth_flags[1], globals->eth_flags[2], globals->eth_flags[3]);
    printf("prot_id:\t\t%.2X:%.2X\n", globals->prot_id[0], globals->prot_id[1]);
    printf("src_mac:\t\t%.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n", globals->src_mac[0], globals->src_mac[1], globals->src_mac[2], globals->src_mac[3], globals->src_mac[4], globals->src_mac[5]);
    printf("dest_mac:\t\t%.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n", globals->dest_mac[0], globals->dest_mac[1], globals->dest_mac[2], globals->dest_mac[3], globals->dest_mac[4], globals->dest_mac[5]);
    printf("broadcast8bitMAC:\t%.2X:%.2X:%.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n", globals->broadcast8bitMAC[0], globals->broadcast8bitMAC[1], globals->broadcast8bitMAC[2], globals->broadcast8bitMAC[3], globals->broadcast8bitMAC[4], globals->broadcast8bitMAC[5], globals->broadcast8bitMAC[6], globals->broadcast8bitMAC[7]);
    printf("SNB_THREAD_INTERVAL:\t%u\n", globals->SNB_THREAD_INTERVAL);
    printf("BCN_THREAD_INTERVAL:\t%u\n", globals->BCN_THREAD_INTERVAL);


}
