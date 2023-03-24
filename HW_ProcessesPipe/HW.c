#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>

int main() {
    int pipe1[2], pipe2[2], pipe3[2], pipe4[2];

    if (pipe(pipe1) == -1) {
        err(EXIT_FAILURE, "pipe1");
    }
    if (pipe(pipe2) == -1) {
        err(EXIT_FAILURE, "pipe2");
    }
    if (pipe(pipe3) == -1) {
        err(EXIT_FAILURE, "pipe3");
    }
    if (pipe(pipe4) == -1) {
        err(EXIT_FAILURE, "pipe4");
    }

    pid_t pid1 = fork();
    if (pid1 == -1) {
        err(EXIT_FAILURE, "fork1");
    }
    if (pid1 == 0) {
        // Child 1: execute "cut -d : -f 7 /etc/passwd"
        close(pipe1[0]);
        close(pipe2[0]);
        close(pipe2[1]);
        close(pipe3[0]);
        close(pipe3[1]);
        close(pipe4[0]);
        close(pipe4[1]);
        dup2(pipe1[1], STDOUT_FILENO);
        execlp("cut", "cut", "-d", ":", "-f", "7", "/etc/passwd", NULL);
        err(EXIT_FAILURE, "exec1");
    }

    pid_t pid2 = fork();
    if (pid2 == -1) {
        err(EXIT_FAILURE, "fork2");
    }
    if (pid2 == 0) {
        // Child 2: execute "sort"
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe3[0]);
        close(pipe3[1]);
        close(pipe4[0]);
        close(pipe4[1]);
        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);
        execlp("sort", "sort", NULL);
        err(EXIT_FAILURE, "exec2");
    }

    pid_t pid3 = fork();
    if (pid3 == -1) {
        err(EXIT_FAILURE, "fork3");
    }

    if (pid3 == 0) {
        // Child 3: execute "uniq"
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[1]);
        close(pipe3[0]);
        close(pipe4[0]);
        close(pipe4[1]);
        dup2(pipe2[0], STDIN_FILENO);
        dup2(pipe3[1], STDOUT_FILENO);
        execlp("uniq", "uniq", NULL);
        err(EXIT_FAILURE, "exec3");
    }

    pid_t pid4 = fork();
    if (pid4 == -1) {
        err(EXIT_FAILURE, "fork4");
    }

    if (pid4 == 0) {
        // Child 4: execute "wc -l"
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        close(pipe3[1]);
        close(pipe4[0]);
        dup2(pipe3[0], STDIN_FILENO);
        dup2(pipe4[1], STDOUT_FILENO);
        execlp("wc", "wc", "-l", NULL);
        err(EXIT_FAILURE, "exec4");
    }

    // Parent
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);
    close(pipe3[0]);
    close(pipe3[1]);
    close(pipe4[1]);

    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    waitpid(pid3, &status, 0);
    waitpid(pid4, &status, 0);

    char buf[1024];
    ssize_t n = read(pipe4[0], buf, sizeof(buf));
    if (n == -1) {
        err(EXIT_FAILURE, "read");
    }
    write(STDOUT_FILENO, buf, n);

    return 0;
}