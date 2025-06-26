#ifndef DISK_H
#define DISK_H

#include <stdio.h>
#include <stdint.h>
#include "common.h"
#include "bitmap.h"
#include "metablock.h"

typedef struct
{
    FILE *file_ptr;
    char path[256];
    size_t size;
    Metablock *metablock;
    Bitmap *bitmap;
} Disk;

Disk *init_disk(char path[256], uint32_t size);
Disk *format_disk(Disk *disk, char drive_name[32]);

Disk *mount_disk(char path[256]);
void unmount_disk(Disk *disk);

int verify_disk(Disk *disk);
Metablock *load_metablock(Disk *disk);

#endif