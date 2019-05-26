#include "cache.h"

void free_cache(cache_t *cache, int iterations){
	for (;iterations > 0; iterations--){
		free(cache -> vias[iterations-1]);
	}
	free(cache -> vias);
}

bool init_cache(cache_t *cache){
	cache -> vias = malloc(N_VIAS*(sizeof(block_t*)));
	if (!cache -> vias) return false;
	
	for (int i = 0; i < N_VIAS; i++){
		(cache -> vias)[i] = malloc(N_BLOCKS*(sizeof(block_t)));
		if (!(cache -> vias)[i]){
			free_cache(cache, i);
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