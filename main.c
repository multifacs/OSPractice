#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    char *commands[][4] = {
        {"ls","/dev", NULL},
        {"sort", NULL},
        {"more", NULL},
    };

    size_t i;
    int prev_pipe, pfds[2];

    prev_pipe = STDIN_FILENO;

    for (i = 0; i < 2; i++) {
        pipe(pfds);

        if (fork() == 0) {
            // Redirect previous pipe to stdin
            if (prev_pipe != STDIN_FILENO) {
                dup2(prev_pipe, STDIN_FILENO);
                close(prev_pipe);
            }

            // Redirect stdout to current pipe
            dup2(pfds[1], STDOUT_FILENO);
            close(pfds[1]);

            // Start command
            execvp(commands[i][0], commands[i]);

            perror("execvp failed");
            exit(1);
        }

        // Close read end of previous pipe (not needed in the parent)
        close(prev_pipe);

        // Close write end of current pipe (not needed in the parent)
        close(pfds[1]);

        // Save read end of current pipe to use in next iteration
        prev_pipe = pfds[0];
    }

    // Get stdin from last pipe
    if (prev_pipe != STDIN_FILENO) {
        dup2(prev_pipe, STDIN_FILENO);
        close(prev_pipe);
    }

    // Start last command
    execvp(commands[i][0], commands[i]);

    perror("execvp failed");
    exit(1);
}
