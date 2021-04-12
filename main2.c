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

    pipe(pfds);

    if (fork() == 0) {
        dup2(pfds[1], STDOUT_FILENO);
        close(pfds[1]);

        execvp(commands[0][0], commands[0]);
    }

    close(prev_pipe);
    close(pfds[1]);

    prev_pipe = pfds[0];

    pipe(pfds);

    if (fork() == 0) {

        dup2(prev_pipe, STDIN_FILENO);
        close(prev_pipe);

        dup2(pfds[1], STDOUT_FILENO);
        close(pfds[1]);

        execvp(commands[1][0], commands[1]);
    }

    close(prev_pipe);
    close(pfds[1]);

    prev_pipe = pfds[0];

    dup2(prev_pipe, STDIN_FILENO);
    close(prev_pipe);

    execvp(commands[2][0], commands[2]);
}
