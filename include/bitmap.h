#ifndef BITMAP_H
#define BITMAP_H
#include <stdint.h>

typedef struct
{
    uint32_t num_elems;
    uint32_t num_blocks;
    uint32_t *bitmap;
} Bitmap;

Bitmap *init_bitmap(uint32_t num_blocks_on_disk, uint32_t block_size);
void free_bitmap(Bitmap *bitmap);
void mark_block_used(Bitmap *bitmap, uint32_t i);

// allocate block
// free block
// get free block
#endif