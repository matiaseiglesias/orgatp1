#ifndef __CACHE_H__
#define __CACHE_H__

#include <stdlib.h>

#include "block.h"
#include "cola.h"

#define N_VIAS 4
#define N_BLOCKS 8
#define MEMORY_SIZE 65536 //en bytes
#define TAG 0xFE00
#define INDEX 0x1C0
#define OFFSET 0x3F

typedef struct {
    block_t **vias;
    cola_t **fifos; //ver si va, según la consigna se debe deducir con los metadatos

	unsigned char* memory; //pongo acá la memoria principal para facilitar acceso

    unsigned int n_acces;
    unsigned int n_miss;
} cache_t;

cache_t* cache; //cache como variable global

bool init_cache();
void delete_cache();
unsigned int get_tag (unsigned int address);
unsigned int get_index (unsigned int address);
unsigned int get_offset (unsigned int address);

unsigned int find_set(unsigned int address);
float get_miss_rate();
void read_tocache(unsigned int blocknum, unsigned int way,
				 unsigned int set);
void write_tocache(unsigned int address, unsigned char);
unsigned char read_byte(unsigned int address);
unsigned int select_oldest(unsigned int setnum);
void write_byte(unsigned int address, unsigned char value);


/*
void init();
unsigned int get_offset (unsigned int address);
void write_byte(unsigned int address, unsigned char value);
*/


#endif /* __CACHE_H__ */