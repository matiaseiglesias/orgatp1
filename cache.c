#include "cache.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void free_cache(int iterations){
	for (;iterations > 0; iterations--){
		free(cache -> vias[iterations-1]);
	}
	free(cache -> vias);
	free(cache -> memory);
}

void free_colas(size_t iterations){
	for (; iterations > 0; iterations--){
		void (*destruction_f)(void*) = free;
		cola_destruir(cache->fifos[iterations-1], destruction_f);
	}
	free(cache->fifos);
}

bool init(){
	cache -> vias = malloc(N_VIAS*(sizeof(block_t*)));
	if (!cache -> vias) return false;
	
	cache -> memory = (unsigned char*) malloc(MEMORY_SIZE);
	if (!(cache->memory)){
		free_cache(0);
		return false;
	}

    memset(cache->memory,0 ,MEMORY_SIZE);

	for (int i = 0; i < N_VIAS; i++){
		(cache -> vias)[i] = malloc(N_BLOCKS*(sizeof(block_t)));
		if (!(cache -> vias)[i]){
			free_cache(i);
			return false;
		}
		for (int j = 0; j < N_BLOCKS; j++){
			block_init(&(cache -> vias)[i][j]);
		}
	}

	//inicio las colas "fifo"
	cache->fifos = malloc(N_BLOCKS*(sizeof(cola_t*)));
	if (!cache->fifos){
		free_cache(N_VIAS);
		return false;	
	} 

	for (size_t i = 0; i < N_BLOCKS; i++){
		(cache->fifos)[i] = cola_crear();
		if (!(cache->fifos)[i]){
			free_colas(i);
			free_cache(N_VIAS);
			return false;
		}
	}
	
	cache -> n_acces = 0;
	cache -> n_miss = 0;
	return true;
}

void delete_cache(){
	free_cache(N_VIAS);
	free_colas(N_BLOCKS);
}

void fifo_update(unsigned int set_num, unsigned int n_via){
	unsigned int *via = malloc(sizeof(unsigned int));
	*via = n_via;
	cola_t *fifo = (cache -> fifos)[set_num];
	cola_encolar(fifo, via);
	//abria que comunicar error en caso de fallar el malloc
}

unsigned int get_tag (unsigned int address){
	return (address & TAG)>>9;
}
unsigned int get_index (unsigned int address){
	return (address & INDEX)>>6;
}
unsigned int get_offset (unsigned int address){
	return (address & OFFSET);
}

bool is_valid_addr(unsigned int address){
	return address < MEMORY_SIZE;
}

unsigned int find_set(unsigned int address){
	return get_index(address);
}

float get_miss_rate(){
	if (cache -> n_acces == 0) return 0;
	return (((float)cache->n_miss / (float)cache->n_acces) * 100);
}

void read_tocache(unsigned int blocknum, unsigned int way,
				 unsigned int set){
	write_block(&(cache->vias[way][set]), &(cache->memory[blocknum]));
}

unsigned int select_oldest(unsigned int set_num){
	cola_t *fifo = (cache -> fifos)[set_num];
	unsigned int *via = (unsigned int*) cola_desencolar(fifo);
	unsigned int n_via = *via;
	free(via);
	return n_via;
}

unsigned char read_byte(unsigned int address){
	if (! is_valid_addr(address)){
		printf("Dirreccion: %u fuera de rango\n", address);
		return ' ';
	}
	unsigned int tag = get_tag(address);
	unsigned int index = get_index(address);
	unsigned int offset = get_offset(address);

	cache->n_acces++;
	int empty_block = -1;
	for (int i = 0; i< N_VIAS; i++){
		if (!is_valid(&(cache->vias[i][index]))){
			empty_block = i;
			continue;
		}  
		if (tag_compare(&(cache->vias[i][index]), tag)){
			return read_byte_b(&(cache->vias[i][index]), offset);
		}//hit
	}//miss
	cache -> n_miss++;
	unsigned int n_via;
	if (empty_block > 0){
		n_via = empty_block;
	} else{
		n_via = select_oldest(index);
	}
	block_t* block_to_replace = &cache->vias[n_via][index];
	write_block(block_to_replace, &(cache->memory[address]));
	set_tag(block_to_replace, tag);

	fifo_update(index, n_via);

	return read_byte_b(block_to_replace, offset);
}

void write_byte(unsigned int address, unsigned char value){
	if (! is_valid_addr(address)){
		printf("Dirreccion: %u fuera de rango\n", address);
		return;
	}
	cache->memory[address] =  value;
	unsigned int tag = get_tag(address);
	unsigned int index = get_index(address);
	unsigned int offset = get_offset(address);

	cache->n_acces++;
	int empty_block = -1;
	for (int i = 0; i< N_VIAS; i++){
		if (!is_valid(&(cache->vias[i][index]))){
			empty_block = i;
			continue;
		}  
		if (tag_compare(&(cache->vias[i][index]), tag)){
			write_byte_b(&(cache->vias[i][index]), offset, value);
			return;
		}//hit
	}//miss
	
	cache -> n_miss++;
	unsigned int n_via;
	if (empty_block > 0){
		n_via = empty_block;
	} else{
		n_via = select_oldest(index);
	}
	block_t* block_to_replace = &cache->vias[n_via][index];
	write_block(block_to_replace, &(cache->memory[address]));
	set_tag(block_to_replace, tag);

	fifo_update(index, n_via);

}