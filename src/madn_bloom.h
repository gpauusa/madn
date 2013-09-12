/*
 ============================================================================
 Name        : bloom.h
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Bloom Header
 ============================================================================
 */

#ifndef __BLOOM_H__
#define __BLOOM_H__

#include <stdlib.h>
#include "madn_main_struct.h"

//#define BLOOM_SIZE 16  defined as MADN_BLOOM_DIM in madn_main_struct

char *bloom_create(size_t size);        //constructor
void bloom_destroy(char **bloom);         //destructor
void bloom_reset(char *bloom, size_t size); //all 0s
void bloom_add_node(BLOOM_HASH *hash, char *bloom, size_t size, NODEID id);
void bloom_add(BLOOM_HASH* hash, char *bloom, size_t size, const char *s);
int bloom_check_node(BLOOM_HASH *hash, char *bloom, size_t size, NODEID id);
int bloom_check(BLOOM_HASH* hash, char *bloom, size_t size, const char *s);
uint32_t bloom_gossip(char *blm1, char *blm2);
BLOOM_HASH* bloom_hash_create(size_t nfuncs, ...); //constructor
void bloom_hash_destroy(BLOOM_HASH** hash);               //destructor

unsigned int sax_hash(const char *key);
unsigned int sdbm_hash(const char *key);

void nodeidToString(char* id_str, NODEID id);
#endif
