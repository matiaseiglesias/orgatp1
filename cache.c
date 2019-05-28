#include "cache.h"
#include <stdlib.h>

void free_cache(cache_t *cache, int iterations){
	for (;iterations > 0; iterations--){
		free(cache -> vias[iterations-1]);
	}
	free(cache -> vias);
	free(cache->memory);
}

void free_colas(cache_t *cache, size_t iterations){
	for (; iterations > 0; iterations--){
		cola_destruir(cache->fifos[iterations-1],NULL);
	}
	free(cache->fifos);
	free_cache(cache, N_VIAS);
}

bool init_cache(cache_t *cache){
	cache-> vias = malloc(N_VIAS*(sizeof(block_t*)));
	if (!cache -> vias) return false;
	
	cache->memory = (unsigned char*) malloc(MEMORY_SIZE);
	if (!(cache->memory)){
		free_cache(cache, 0);
		return false;
	}

    memset(cache->memory,0 ,MEMORY_SIZE);

	for (int i = 0; i < N_VIAS; i++){
		(cache -> vias)[i] = malloc(N_BLOCKS*(sizeof(block_t)));
		if (!(cache -> vias)[i]){
			free_cache(cache, i);
			return false;
		}
	}

	//inicio las colas "fifo"
	cache->fifos = malloc(N_BLOCKS*(sizeof(cola_t*)));
	if (!cache->fifos){
		free_cache(cache, N_VIAS);
		return false;	
	} 

	for (size_t i = 0; i < N_BLOCKS; i++){
		(cache->fifos)[i] = cola_crear();
		if (!(cache->fifos)[i]){
			free_colas(cache->fifos, N_VIAS);
			return false;
		}
	}
	
	cache -> n_acces = 0;
	cache -> n_miss = 0;
	return true;
}

void delete_cache(cache_t *cache){
	free_cache(cache, N_VIAS);
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

unsigned int find_set(unsigned int address){
	return get_index(address);
}

float get_miss_rate(){
	return (((float)cache->n_miss / (float)cache->n_acces) * 100);
}

void read_tocache(unsigned int blocknum, unsigned int way, unsigned int set){
	write_block(&(cache->vias[way][set]), cache->memory[blocknum]);
}

unsigned char read_byte(unsigned int address){
	unsigned int tag = get_tag(address);
	unsigned int index = get_index(address);
	unsigned int offset = get_offset(address);

	block_t* block_via1 = &cache->vias[0][index];
	block_t* block_via2 = &cache->vias[1][index];
	block_t* block_via3 = &cache->vias[2][index];
	block_t* block_via4 = &cache->vias[3][index];

	cache->n_acces++;

	if (block_via1->v == 1) {
		if (tag == block_via1->tag) {
			// hit en esta via
			return (unsigned char)block_via1->data[offset];
		}
		// miss en esta via, puede estar en otras
	}
 
	if (block_via2->v == 1) {
		if (tag == block_via2->tag) {
			// hit
            return (unsigned char)block_via2->data[offset];
		}
		// miss en esta via
	}


	if (block_via3->v == 1) {
		if (tag == block_via3->tag) {
			// hit
            return (unsigned char)block_via3->data[offset];
		}
		// miss en esta via
	}


	if (block_via4->v == 1) {
		if (tag == block_via4->tag) {
			// hit
            return (unsigned char)block_via4->data[offset];
		}
		// miss en esta via hay que cargar el bloque a cache
	}

	cache->n_miss++;
	int n_via = select_oldest(index);
	block_t* block_to_replace = &cache->vias[n_via][index];

	write_block(block_to_replace, cache->memory[address]);
	block_to_replace->v = 1;
	block_to_replace->tag = tag;

	return block_to_replace->data[offset]; //ver si esto está bien, en general toda la función
}
