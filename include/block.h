#ifndef BLOCK_H
#define BLOCK_H

#include "disk.h"
#include <stdint.h>

uint8_t *read_block(Disk *disk, uint32_t block_number);
void write_block(Disk *disk, uint32_t block_number, uint8_t *data, size_t data_size);
void free_block_buffer(uint8_t *block_buffer);
void clear_block(Disk *disk, uint32_t block_number);

void write_metablock(Disk *disk);
void write_bitmap(Disk *disk);

#endif