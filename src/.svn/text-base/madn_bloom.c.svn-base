/*
 * bloom.c
 *
 *  Created on: Jan 19, 2011
 *      Author: fabio
 */

#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "madn_bloom.h"

#define SETBIT(a, n) (a[n/CHAR_BIT] |= (1<<(n%CHAR_BIT)))
#define GETBIT(a, n) (a[n/CHAR_BIT] & (1<<(n%CHAR_BIT)))


char *bloom_create(size_t size)
{
    char *bloom;

    if(!(bloom =calloc((size+CHAR_BIT-1)/CHAR_BIT, sizeof(char)))) {
        //free(bloom); //TODO if not allocated why free???
        return NULL;
    }

    return bloom;
}

BLOOM_HASH* bloom_hash_create(size_t nfuncs, ...)
{
    va_list l;
    int n;
    BLOOM_HASH* hash;

    if(!(hash=malloc(sizeof(BLOOM_HASH)))) assert(0);
    if(!(hash->funcs=(hashfunc_t*)malloc(nfuncs*sizeof(hashfunc_t)))) {
        bloom_hash_destroy(&hash);
        assert(0);
    }
    assert (hash != NULL);

    va_start(l, nfuncs);
    for(n=0; n<nfuncs; ++n) {
        hash->funcs[n]=va_arg(l, hashfunc_t);
    }
    va_end(l);
    hash->nfuncs=nfuncs;

    return hash;
}

void bloom_hash_destroy(BLOOM_HASH **hash)
{
    free((*hash)->funcs);
    free(*hash);
    *hash = NULL;
}

void bloom_destroy(char **bloom)
{
    free(*bloom);
    *bloom = NULL;
}

void bloom_reset(char *bloom, size_t size)
{
    memset(bloom, 0, size);
}

void bloom_allset(char *bloom, size_t size)
{
    memset(bloom, 1, size);
}

void nodeidToString(char* id_str, NODEID id)
{
    //uint32_t id = 0;
    memset(id_str, 0, 15);
    uint8_t i = 0;
    uint32_t num = 1000000000;
    while (num != 0)
    {
        uint8_t digit = id / num;
        //printf("%d", digit);
        id = id % num;
        num = num / 10;
        if (digit == 0 && i == 0) continue;
        id_str[i++] = 48 + digit;
    }
    if (i == 0) id_str[i++] = 48;
}

void bloom_add_node(BLOOM_HASH *hash, char *bloom, size_t size, NODEID id)
{
    char id_str[15];
    nodeidToString(id_str, id);
    bloom_add(hash, bloom, size, id_str);
}

void bloom_add(BLOOM_HASH *hash, char *bloom, size_t size, const char *s)
{
    size_t n;
    int pos;
    for(n=0; n<hash->nfuncs; ++n) {
        //printf("%d\n", hash->funcs[n](s));
        pos = hash->funcs[n](s) % size;
        SETBIT(bloom, pos);
    }
}

int bloom_check_node(BLOOM_HASH *hash, char *bloom, size_t size, NODEID id)
{
    char id_str[15];
    nodeidToString(id_str, id);
    return bloom_check(hash, bloom, size, id_str);
}

int bloom_check(BLOOM_HASH *hash, char *bloom, size_t size, const char *s)
{
    size_t n;
    int pos;

    for(n=0; n<hash->nfuncs; ++n) {
        pos = hash->funcs[n](s)% size;
        if(!(GETBIT(bloom, pos))) return 0;
    }
    return 1;
}

uint32_t bloom_gossip(char *blm1, char *blm2)
{
    uint32_t* bloom1 = (uint32_t*) blm1;
    uint32_t* bloom2 = (uint32_t*) blm2;
    //bloom2 is the subset filter
    uint32_t tmp[BLOOM_INT_SIZE];
    uint32_t rst=0;
    uint8_t pos = 0;

    while(pos < BLOOM_INT_SIZE) {
        tmp[pos] = (bloom1[pos] & bloom2[pos]) ^ bloom2[pos];
        pos++;
    }
    pos = 0;
    while(pos < BLOOM_INT_SIZE) 
        rst = rst + tmp[pos++];
    return rst;
}

unsigned int sax_hash(const char *key)
{
    unsigned int h=0;
    while(*key) h^=(h<<5)+(h>>2)+(unsigned char)*key++;
    return h;
}

unsigned int sdbm_hash(const char *key)
{
    unsigned int h=0;
    while(*key) h=(unsigned char)*key++ + (h<<6) + (h<<16) - h;
    return h;
}

