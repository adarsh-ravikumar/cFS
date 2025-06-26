#include "metablock.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common.h"

Metablock *init_metablock(size_t disk_size, char drive_name[32])
{
    Metablock *metablock = malloc(sizeof(Metablock));
    if (!metablock)
    {
        fprintf(stderr, "Failed to allocate memory for metablock!\n");
    }
    metablock->magic_number = MAGIC_NUMBER;
    metablock->block_size = DEFAULT_BLOCK_SIZE;
    metablock->num_blocks = disk_size / DEFAULT_BLOCK_SIZE;
    strcpy(metablock->drive_name, drive_name);

    return metablock;
}

Metablock *set_start(Metablock *metablock, uint32_t bitmap_start, uint32_t inode_start, uint32_t data_start)
{
    metablock->bitmap_start = bitmap_start;
    metablock->inode_start = inode_start;
    metablock->data_start = data_start;

    return metablock;
}