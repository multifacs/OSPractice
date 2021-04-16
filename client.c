#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shared_memory.h"

#define BLOCK_SIZE 4096

int main(int argc, char *argv[])
{
    // if (argc != 2) {
    //     printf("usage - %s [stuff to write]", argv[0]);
    //     return -1;
    // }

    if (argc != 1) {
        printf("usage - %s //no args", argv[0]);
        return -1;
    }

    printf("Writing...\n");

    char *block = attach_memory_block(FILENAME, BLOCK_SIZE);
    if (block == NULL) {
        printf("ERROR: couldn't get block\n");
        return -1;
    }

    //printf("Writing: \"%s\"\n", argv[1]);
    //strncpy(block, argv[1], BLOCK_SIZE);

    int i, j;
    for (i = 0; i < 3; i++) {
        char msg = 'a' + i;
        
        for (j = 0; j < 3; j++)
            block[1+j] = msg;
        
        block[0] += 3;
        sleep(1);
    }

    block[0] = -1;

    detach_memory_block(block);

    return 0;
}