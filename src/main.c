#include <stdio.h>
#include "disk.h"
#include "utils.h"


// test routines
void init_fmt_disk() {
    printf("Initializing disk....\n");
    // initialize a disk
    Disk *disk = init_disk("my_disk.img", MB(100));

    // format it
    printf("Done! Formatting it...\n");
    disk = format_disk(disk, "My Pretty Disk");

    // unmount it    
    printf("Done! Time to unmount!\n");
    unmount_disk(disk);
}   


int main() {
    init_fmt_disk();
    return 0;
}