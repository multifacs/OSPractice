#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    size_t i;
    int prev_pipe, pfds[2];

    prev_pipe = STDIN_FILENO;

    pipe(pfds);
    if (fork() == 0) {
        // Redirect stdout to current pipe
        dup2(pfds[1], STDOUT_FILENO);
        close(pfds[1]);
        // Start command
        execvp("ls", {"ls","/dev", NULL});
    }
    // Close read end of previous pipe (not needed in the parent)
    close(prev_pipe);
    // Close write end of current pipe (not needed in the parent)
    close(pfds[1]);
    // Save read end of current pipe to use in next iteration
    prev_pipe = pfds[0];



    pipe(pfds);
    if (fork() == 0) {
        // Redirect previous pipe to stdin
        dup2(prev_pipe, STDIN_FILENO);
        close(prev_pipe);

        // Redirect stdout to current pipe
        dup2(pfds[1], STDOUT_FILENO);
        close(pfds[1]);
        // Start command
        execvp("sort", {"sort", NULL});
    }
    // Close read end of previous pipe (not needed in the parent)
    close(prev_pipe);
    // Close write end of current pipe (not needed in the parent)
    close(pfds[1]);
    // Save read end of current pipe to use in next iteration
    prev_pipe = pfds[0];


    // Start last command
    execvp("more", {"more", NULL});
}
