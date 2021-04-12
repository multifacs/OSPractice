#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    size_t i;
    int prev_pipe, pfds[2];

    prev_pipe = STDIN_FILENO;

    pipe(pfds);
    int pid1 = fork();

    if (pid1 == 0) {
        // Redirect stdout to current pipe
        dup2(pfds[1], STDOUT_FILENO);
        close(pfds[1]);
        // Start command
        execlp("ls", "ls", "/dev", NULL);
    }
    // Close read end of previous pipe (not needed in the parent)
    close(prev_pipe);
    // Close write end of current pipe (not needed in the parent)
    close(pfds[1]);
    // Save read end of current pipe to use in next iteration
    prev_pipe = pfds[0];



    pipe(pfds);
    int pid2 = fork();
    if (pid2 == 0) {
        // Redirect previous pipe to stdin
        dup2(prev_pipe, STDIN_FILENO);
        close(prev_pipe);

        // Redirect stdout to current pipe
        dup2(pfds[1], STDOUT_FILENO);
        close(pfds[1]);
        // Start command
        execlp("sort", "sort", NULL);
    }
    // Close read end of previous pipe (not needed in the parent)
    close(prev_pipe);
    // Close write end of current pipe (not needed in the parent)
    close(pfds[1]);


    // Start last command
    execlp("more", "more", NULL);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return 0;
}
