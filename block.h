#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <stdbool.h>

#define BS 64

typedef struct {
    char data[BS];
    char tag;
    bool dirty;
    unsigned int v;
} block_t;

void write_block(block_t *block, char *mem_block);
char read_byte_b(block_t *block, unsigned int offset);
void write_byte_b(block_t *block, unsigned int offset, unsigned char value);
bool tag_compare(block_t *block, char tag);
bool is_dirty(block_t *block);
void set_dirty(block_t *block);
void set_clean(block_t *block);

#endif /* __BLOCK_H__ */