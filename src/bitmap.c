#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bitmap.h"
#include "utils.h"
#include "math.h"

#define BITMAP_START 1
#define BITMAP_END (BITMAP_START + bitmap->num_blocks - 1)

Bitmap *init_bitmap(uint32_t num_blocks_on_disk, uint32_t block_size)
{
    Bitmap *bitmap = malloc(sizeof(Bitmap));
    
    if (!bitmap) {
        fprintf(stderr, "Failed to allocate memory for Bitmap struct.\n");
        return NULL;
    }
    
    bitmap->num_elems = (num_blocks_on_disk + 31) / 32;
    bitmap->num_blocks = (uint32_t)ceil((double)(bitmap->num_elems * sizeof(uint32_t)) / block_size);
    
    
    bitmap->bitmap = (uint32_t *)calloc(bitmap->num_elems, sizeof(uint32_t));
    
    if (!bitmap->bitmap) {
        fprintf(stderr, "Failed to allocate bitmap array.\n");
        return NULL;
    }
    
    
    for (size_t i = BITMAP_START; i <= BITMAP_START + bitmap->num_blocks - 1; i++)
    {
        mark_block_used(bitmap, i);
    }
    
    return bitmap;
}

void mark_block_used(Bitmap *bitmap, uint32_t block_index)
{
    uint32_t word_index = block_index / 32;
    uint32_t bit_offset = block_index % 32;

    if (word_index >= bitmap->num_elems) {
        fprintf(stderr, "mark_block_used: block index out of range (%u)\n", block_index);
        return;
    }

    bitmap->bitmap[word_index] |= (1U << bit_offset);
}

void free_bitmap(Bitmap *bitmap)
{
    free(bitmap->bitmap);
}