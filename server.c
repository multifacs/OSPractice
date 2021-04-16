#include <string.h>

#include "shared_memory.h"

int main(int argc, char *argv[])
{
    if (argc != 1) {
        printf("usage - %s //no args", argv[0]);
        return -1;
    }

    char *block = attach_memory_block(FILENAME, BLOCK_SIZE);
    if (block == NULL) {
        printf("ERROR: couldn't get block\n");
        return -1;
    }

    block[0] = 0;

    int prev_size = block[0];

    while(block[0] != -1) {
        if (block[0] != prev_size && block[0] != -1) {
            printf("Reading: \"%c%c%c\"\n", block[1], block[2], block[3]);
            printf("Reading: \"%d\"\n", block[0]);
            prev_size = block[0];
        }
    }

    //printf("Reading: \"%s\"\n", block);
    //printf("Reading: \"%d\"\n", block[0]);

    detach_memory_block(block);
    destroy_memory_block(block);

    return 0;
}