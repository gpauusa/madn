#ifndef __MADN_MAIN_STRUCT_H__
#define __MADN_MAIN_STRUCT_H__

#include <stdint.h>
#include <stdio.h>
#include "MSRStructs.h"
#include "madn_settings.h"

typedef char* MADN_PTR; 

typedef unsigned int (*hashfunc_t)(const char *);   //from bloom.h

typedef struct {
        size_t nfuncs;
        hashfunc_t *funcs;
} BLOOM_HASH;           //from bloom.h

#define PIECES_DIM 32
#define DATA_DIM 1400
#define MADN_BLOOM_DIM 16
#define BLOOM_INT_SIZE MADN_BLOOM_DIM/sizeof(uint32_t)

typedef uint32_t NODEID ; //glib hash table requires 32 bit
typedef uint32_t CID ;
typedef uint32_t SID ;

#define SID_DIM sizeof(SID)
#define CID_DIM sizeof(CID)
#define NODEID_DIM sizeof(NODEID)

typedef uint8_t PIECES[PIECES_DIM];
typedef uint8_t DATA_PAYLOAD[DATA_DIM];
typedef char MADN_BLOOM[MADN_BLOOM_DIM]; //TODO made it char

typedef struct {

        void* mod_rq;       //Request Table
        void* mod_snb;      //Sensed Neighbor Table
        void* mod_sq;       //Service Queue
        void* mod_lq;       //Leech (Request) Queue
        void* mod_fq;       //Forwarding Queue
        void* mod_cache;    //Data Cache
        void* mod_local_cb; //Callback functions for local apps
        void* mod_encode;   //TODO:??
        void* mod_time;     //TODO:??
        BLOOM_HASH *hash;   //from bloom.h
        MADN_BLOOM bloom;   //local bloom filter
        MSRSendInfo* si;
        MSRRecvInfo* ri;
        MADN_SETTINGS* globals;
} MADN_INSTANCE;

typedef struct {
    CID chunk;
    SID stripe;
} MADN_DATAID;

#define DATAID_DIM sizeof(MADN_DATAID)

MADN_INSTANCE* create_madn_instance();
MADN_BLOOM* create_madn_bloom();
CID* create_cid();
SID* create_sid();
MADN_DATAID* create_dataid();
PIECES* create_pieces();

void free_madn_bloom(MADN_BLOOM** tmp);
void free_cid(CID** tmp);
void free_sid(SID** tmp);
void free_dataid(MADN_DATAID** tmp);
void free_pieces(PIECES** tmp);

int cmp_cid(CID* p1, CID* p2);
int cmp_sid(SID* p1, SID* p2);
int cmp_dataid(MADN_DATAID* p1, MADN_DATAID* p2);
int cmp_bloom(MADN_BLOOM* p1, MADN_BLOOM* p2);
int cmp_pieces(PIECES* p1, PIECES* p2);

void dif_pieces(PIECES* p1, PIECES* p2, PIECES* p3);
void complementary_pieces(PIECES* p1, PIECES* p2);
uint32_t collapse_pieces(PIECES *p);

void copy_cid(CID* p1 , CID* p2);
void copy_sid(SID* p1 , SID* p2);
void copy_pieces(PIECES* p1 , PIECES* p2);
void copy_dataid(MADN_DATAID* p1 , MADN_DATAID* p2);
void copy_bloom(MADN_BLOOM* p1, MADN_BLOOM* p2);

void fprint_nodeid(FILE* f, MADN_PTR pkt);
void fprint_cid(FILE *f, MADN_PTR pkt);
void fprint_sid(FILE *f, MADN_PTR pkt);
void fprint_dataid(FILE *f, MADN_PTR pkt);
void fprint_pieces(FILE *f, MADN_PTR pkt);
void fprint_bloom(FILE *f, MADN_PTR pkt);

void fprint_field_8(FILE *f, MADN_PTR pkt, int len);
void fprint_field_16(FILE *f, MADN_PTR pkt, int len);
void fprint_field_32(FILE *f, MADN_PTR pkt, int len);
#endif
