#include "madn_main_struct.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/** CONSTRUCTOR FIELD SECTION */

MADN_INSTANCE* create_madn_instance()
{
    MADN_INSTANCE* tmp = (MADN_INSTANCE*) calloc(1,sizeof(MADN_INSTANCE));
    tmp->globals = create_madn_settings();
    return tmp;
}

MADN_BLOOM* create_madn_bloom()
{
    MADN_BLOOM* tmp = (MADN_BLOOM*) calloc(1,MADN_BLOOM_DIM);
    return tmp;
}

CID* create_cid()
{ 
    CID* tmp = (CID*) calloc(1,CID_DIM);
    return tmp;
}

SID* create_sid()
{
    SID* tmp = (SID*) calloc(1,SID_DIM);
    return tmp;
}

MADN_DATAID* create_dataid()
{
    MADN_DATAID* tmp = (MADN_DATAID*) calloc(1,DATAID_DIM);
    return tmp;
}

PIECES* create_pieces()
{
    PIECES* tmp = (PIECES*) calloc(1,PIECES_DIM);
    return tmp;
}

/** DESTRUCTORS FIELD SECTION */

void free_madn_bloom(MADN_BLOOM** tmp)
{
   free(*tmp);
   *tmp = NULL;
}

void free_cid(CID** tmp)
{
   free(*tmp);
   *tmp = NULL;
}

void free_sid(SID** tmp)
{
   free(*tmp);
   *tmp = NULL;
}

void free_dataid(MADN_DATAID** tmp)
{
   free(*tmp);
   *tmp = NULL;
}

void free_pieces(PIECES** tmp)
{
   free(*tmp);
   *tmp = NULL;
}

/** COMPARE FIELD SECTION */

int cmp_cid(CID* p1, CID* p2)
{
    return memcmp(p1, p2, CID_DIM);
}

int cmp_sid(SID* p1, SID* p2)
{
    return memcmp(p1, p2, SID_DIM);
}

int cmp_dataid(MADN_DATAID* p1, MADN_DATAID* p2)
{
    return memcmp(p1, p2, DATAID_DIM);
}

int cmp_bloom(MADN_BLOOM* p1, MADN_BLOOM* p2)
{
    return memcmp(p1, p2, MADN_BLOOM_DIM);
}

int cmp_pieces(PIECES* p1, PIECES* p2)
{
    return memcmp(p1, p2, PIECES_DIM);
}


/** OPERATIONS ON PIECES MAPS*/

void dif_pieces(PIECES* p1, PIECES* p2, PIECES* p3)
{
    int i = 0;
    //int dim = sizeof(PIECES_DIM) / sizeof(int);
    // IT HAS TO BE DIVISIBLE FOR sizeof(int)
    
    while (i < PIECES_DIM)
    {
        (*p3)[i] = (*p1)[i] & ~(*p2)[i];
        //printf("%d-", *p1[i]);
        i++;
    }
    //printf("\n");
}

void complementary_pieces(PIECES* p1, PIECES* p3)
{
    int i = 0;
    //int dim = sizeof(PIECES_DIM) / sizeof(int);
    // BLOOM HAS TO BE DIVISIBLE FOR sizeof(int)

    while (i < PIECES_DIM)
    {
        (*p3)[i] = ~(*p1)[i];
        //printf("%d-", (*p1)[i]);
        i++;
    }
    //printf("\n");
}

uint32_t collapse_pieces(PIECES *p)
{
    uint32_t ret = 0;
    int i;
    int mult_factor = (sizeof(uint32_t)/sizeof(uint8_t));
    for (i = 0; i < PIECES_DIM/mult_factor; i++)
    {
        //printf("DBG: %d: num is %u ret is %u\n", i, * ( (uint32_t*) (p) + i) , ret);
        ret ^= * ( ( (uint32_t*) p) + i);
    }
    return ret;
}

/** COPY FIELD SECTION */

void copy_cid(CID* p1, CID* p2)
{
    memcpy(p1, p2, CID_DIM);
}

void copy_sid(SID* p1, SID* p2)
{
    memcpy(p1, p2, SID_DIM);
}

void copy_pieces(PIECES* p1, PIECES* p2)
{
    memcpy(p1, p2, PIECES_DIM);
}

void copy_dataid(MADN_DATAID* p1, MADN_DATAID* p2)
{
    memcpy(p1, p2, DATAID_DIM);
}

void copy_bloom(MADN_BLOOM* p1, MADN_BLOOM* p2)
{
    memcpy(p1, p2, MADN_BLOOM_DIM);
}

/** PRINTING DEBUG SECTION */

void fprint_field_32(FILE *f, MADN_PTR pkt, int len)
{
    int i; 
    for (i = 0; i < len; i = i + 4)
    {
        fprintf(f, "%#.8x ", *((uint32_t*)pkt + i));
    }
}

void fprint_field_16(FILE *f, MADN_PTR pkt, int len)
{
    int i; 
    for (i = 0; i < len; i = i + 2)
    {
        fprintf(f, "%#.4x ", *((uint32_t*)pkt + i));
    }
}

void fprint_field_8(FILE *f, MADN_PTR pkt, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        fprintf(f, "%#.2x", *((uint8_t*)pkt + i));
    }
}

void fprint_nodeid(FILE *f, MADN_PTR pkt)
{
    fprint_field_16(f, pkt, NODEID_DIM);
}

void fprint_cid(FILE *f, MADN_PTR pkt)
{
    fprint_field_32(f, pkt,  CID_DIM);
}
void fprint_sid(FILE *f, MADN_PTR pkt)
{
    fprint_field_32(f, pkt,  SID_DIM);
}

void fprint_dataid(FILE *f, MADN_PTR pkt)
{
    fprint_field_32(f, pkt,  DATAID_DIM);
}

void fprint_pieces(FILE *f, MADN_PTR pkt)
{
    fprint_field_8(f, pkt,  PIECES_DIM);
}

void fprint_bloom(FILE *f, MADN_PTR pkt)
{
    fprint_field_8(f, pkt, MADN_BLOOM_DIM);
}
