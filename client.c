#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shared_memory.h"

int main(int argc, char *argv[])
{
    if (argc != 1) {
        printf("usage - %s //no args", argv[0]);
        return -1;
    }

    //printf("Writing...\n");

    char *block = attach_memory_block(FILENAME, BLOCK_SIZE);
    if (block == NULL) {
        printf("ERROR: couldn't get block\n");
        return -1;
    }

    //printf("Writing: \"%s\"\n", argv[1]);
    //strncpy(block, argv[1], BLOCK_SIZE);

    int i, j;
    for (i = 0; i < 3; i++) {
        char msg = 'A' + i;
        for (j = block[0]; j < block[0] + 3; j++)
            block[j+1] = msg;
        
        block[0] += 3;
        sleep(2);
    }

    block[0] = -1;

    detach_memory_block(block);

    return 0;
}