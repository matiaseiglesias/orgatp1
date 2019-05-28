#include "block.h"
#include <string.h>

void write_block(block_t *block, char *mem_block){
	memcpy(block -> data, mem_block, BS);
}
char read_byte_b(block_t *block, unsigned int offset){
	return (block -> data)[offset]; 
}
void write_byte_b(block_t *block, unsigned int offset, unsigned char value){
	(block -> data)[offset] = value;
}
bool tag_compare(block_t *block, char tag){
	return block -> tag == tag;
}
bool is_dirty(block_t *block){
	return block -> dirty;
}
void set_dirty(block_t *block){
	block -> dirty = true;
}
void set_clean(block_t *block){
	block -> dirty = false;
}