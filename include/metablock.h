#ifndef METABLOCK_H
#define METABLOCK_H
#include <stdint.h>
#include <stdlib.h>

typedef struct
{
    uint32_t magic_number;
    uint32_t block_size;
    uint32_t num_blocks;
    uint32_t bitmap_start;
    uint32_t inode_start;
    uint32_t inode_size;
    uint32_t data_start;
    char drive_name[32];
    uint32_t root_inode_id;
} Metablock;

Metablock *init_metablock(size_t disk_size, char drive_name[32]);
Metablock *set_start(Metablock *metablock, uint32_t bitmap_start, uint32_t inode_start, uint32_t data_start);
#endif