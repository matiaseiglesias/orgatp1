#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <stdbool.h>

#define BS 64

typedef struct {
    char data[BS];
    unsigned int tag;
    bool valid;
} block_t;

void block_init(block_t *block);
void write_block(block_t *block, unsigned char *mem_block);
char read_byte_b(block_t *block, unsigned int offset);
void write_byte_b(block_t *block, unsigned int offset, unsigned char value);
bool tag_compare(block_t *block, unsigned int tag);
bool is_valid(block_t *block);
void set_valid(block_t *block);
void set_tag(block_t *block, unsigned int tag_);
//void set_clean(block_t *block);

#endif /* __BLOCK_H__ */