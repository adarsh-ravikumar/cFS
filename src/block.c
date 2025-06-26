#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "block.h"
#include "disk.h"

uint8_t *read_block(Disk *disk, uint32_t block_number)
{
    uint32_t block_size = disk->metablock->block_size;

    uint32_t offset = (block_number == 0)
                          ? 0
                          : (block_number - 1) * block_size;

    fseek(disk->file_ptr, offset, SEEK_SET);

    uint8_t *block_buffer = malloc(block_size);
    if (!block_buffer)
    {
        fprintf(stderr, "Failed to create block buffer!\n");
        return NULL;
    }

    size_t items_read = fread(block_buffer, sizeof(uint8_t), block_size, disk->file_ptr);

    if (items_read != block_size)
    {
        fprintf(stderr, "Failed to read block!\n");
        return NULL;
    }

    return block_buffer;
}

void write_block(Disk *disk, uint32_t block_number, uint8_t *data, size_t data_size)
{
    uint32_t block_size = disk->metablock->block_size;

    if (data_size > block_size)
    {
        fprintf(stderr, "Data too large for block!\n");
        return;
    }

    uint32_t offset = block_number * block_size;

    fseek(disk->file_ptr, offset, SEEK_SET);
    if (data_size < block_size)
    {
        uint8_t *temp_buf = calloc(block_size, 1);
        memcpy(temp_buf, data, data_size);

        fwrite(temp_buf, sizeof(uint8_t), block_size, disk->file_ptr);

        free(temp_buf);
    }
    else
    {
        fwrite(data, sizeof(uint8_t), block_size, disk->file_ptr);
    }

    fflush(disk->file_ptr);
}

void write_metablock(Disk *disk)
{
    const Metablock *metablock = disk->metablock;
    write_block(disk, 0, (uint8_t *)(metablock), sizeof(Metablock));
}

void free_block_buffer(uint8_t *block_buffer)
{
    free(block_buffer);
}

void clear_block(Disk *disk, uint32_t block_number)
{
    uint32_t block_size = disk->metablock->block_size;

    uint32_t offset = (block_number == 0)
                          ? 0
                          : (block_number - 1) * block_size;

    fseek(disk->file_ptr, offset, SEEK_SET);

    uint8_t *temp_buf = calloc(block_size, 1);

    fwrite(temp_buf, sizeof(uint8_t), block_size, disk->file_ptr);

    fflush(disk->file_ptr);
}

void write_bitmap(Disk *disk)
{
    Bitmap *bitmap = disk->bitmap;
    size_t start_index = 0;
    size_t offset = disk->metablock->block_size / sizeof(uint32_t);
    for (size_t i = 1; i < bitmap->num_blocks + 1; i++)
    {
        size_t end_index = start_index + offset;

        if (end_index > bitmap->num_elems)
        {
            end_index = bitmap->num_elems;
        }

        size_t chunk_size = end_index - start_index;

        uint32_t *slice = malloc(sizeof(uint32_t) * chunk_size);
        if (!slice)
        {
            fprintf(stderr, "Failed to allocate memory for bitmap slice.\n");
            return;
        }

        memcpy(slice, &bitmap->bitmap[start_index], chunk_size * sizeof(uint32_t));

        write_block(disk, i, (uint8_t *)slice, chunk_size * sizeof(uint32_t));

        free(slice);
        start_index = end_index;
    }
}
