#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "disk.h"
#include "utils.h"
#include "common.h"
#include "bitmap.h"
#include "metablock.h"
#include "block.h"

Disk *init_disk(char path[256], uint32_t size_in_bytes)
{
    FILE *file_ptr = fopen(path, "wb+");
    Disk *disk = malloc(sizeof(Disk));
    if (!disk)
    {
        fprintf(stderr, "Failed to allocate memory for disk!");
    }

    uint8_t *zero_block = calloc(1, DEFAULT_BLOCK_SIZE);

    size_t full_blocks = size_in_bytes / DEFAULT_BLOCK_SIZE;
    size_t remainder = size_in_bytes % DEFAULT_BLOCK_SIZE;

    for (size_t i = 0; i < full_blocks; i++)
    {
        fwrite(zero_block, 1, DEFAULT_BLOCK_SIZE, file_ptr);
    }

    if (remainder > 0)
    {
        fwrite(zero_block, 1, remainder, file_ptr);
    }

    disk->file_ptr = file_ptr;
    disk->size = size_in_bytes;

    free(zero_block);

    return disk;
}

Disk *format_disk(Disk *disk, char drive_name[32])
{

    const size_t size = disk->size;
    fseek(disk->file_ptr, 0, SEEK_SET);

    // initalize the metablock
    Metablock *metablock = init_metablock(size, drive_name);

    // initialize bitmap
    Bitmap *bitmap = init_bitmap(metablock->num_blocks, metablock->block_size);
    mark_block_used(bitmap, 0); // metablock

    set_start(metablock, 0x1, bitmap->num_blocks + 1, 0x0);

    disk->metablock = metablock;
    disk->bitmap = bitmap;

    write_metablock(disk);
    write_bitmap(disk);

    return disk;
}

Disk *mount_disk(char path[256])
{
    FILE *file_ptr = fopen(path, "rb+");
    Disk *disk = malloc(sizeof(Disk));
    if (!disk)
    {
        fprintf(stderr, "Failed to allocate memory for disk!\n");
        return NULL;
    }

    disk->file_ptr = file_ptr;
    snprintf(disk->path, sizeof(disk->path), "%s", path);

    if (!verify_disk(disk))
    {
        fprintf(stderr, "The disk has not been formatted or is corrupt!");
    }

    return disk;
}

void unmount_disk(Disk *disk)
{
    if (disk->file_ptr)
        fclose(disk->file_ptr);

    if (disk->metablock)
        free(disk->metablock);

    if (disk->bitmap)
        free(disk->bitmap->bitmap);
    free(disk->bitmap);

    free(disk);
}

int verify_disk(Disk *disk)
{
    uint32_t magic_number;
    size_t bytes_read = fread(&magic_number, sizeof(uint32_t), 1, disk->file_ptr);
    if (bytes_read == 0)
        return 0;
    if (magic_number != 0xC0FFEE)
        return 0;
    return 1;
}

Metablock *load_metablock(Disk *disk)
{
    fseek(disk->file_ptr, sizeof(uint32_t), SEEK_SET);

    uint32_t block_size;
    size_t items_read = fread(&block_size, sizeof(uint32_t), 1, disk->file_ptr);

    if (items_read != 1)
    {
        block_size = DEFAULT_BLOCK_SIZE;
    }

    fseek(disk->file_ptr, 0, SEEK_SET);

    Metablock *metablock = malloc(sizeof(Metablock));
    if (!metablock)
    {
        fprintf(stderr, "Failed to allocate memory for metablock!\n");
        return NULL;
    }

    items_read = fread(metablock, sizeof(Metablock), 1, disk->file_ptr);
    if (items_read != 1)
    {
        fprintf(stderr, "An unexpected error occurred when loading the metablock!\n");
        free(metablock);
        return NULL;
    }

    return metablock;
}
