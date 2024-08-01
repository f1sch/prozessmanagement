/*
 * Dieses Programm erstellt zwei Kindprozesse mit fork(), die
 * mit execvp() durch "ls -la /tmp" Prozesse ersetzt werden.
 * Der Elternprozess wartet auf die Terminierung der Kinder
 * und terminiert anschließend selbst.
 * */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int main() {
    pid_t pid, wpid;
    if ((pid = fork()) == -1) {
        /* error */
        perror("Fork failed ");
        exit(1);
    } else if (pid == 0) {
        /* child */
        char *arguments[] = {"ls", "-l", "-a", "/tmp", NULL};
        printf("child:  PID [%i] PPID [%i]\n", getpid(), getppid());
        execvp(arguments[0], arguments);
        perror("execvp failed");
        exit(1);
    } else {
        /* parent */
        // does nothing here
    }
    if ((pid = fork()) == -1) {
        /* error */
        perror("Fork failed ");
        exit(1);
    } else if (pid == 0) {
        /* child */
        char *arguments[] = {"ls", "-l", "-a", "/tmp", NULL};
        printf("child:  PID [%i] PPID [%i]\n", getpid(), getppid());
        execvp(arguments[0], arguments);
        perror("execvp failed");
        exit(1);
    } else {
        /* parent */
        int status;
        printf("parent: PID [%i] CPID [%i]\n", getpid(), pid);
        printf("waiting for child ...\n");
        for (int i=0; i<2; i++) {
            /* wait for 2 processes */
            wpid = waitpid(-1, &status, 0);
            if (WIFEXITED(status)) {
                printf("[%i] child (PID: %i) terminated via exit(%d)\n", i, wpid, WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("[%i] child (PID: %i) terminated via signal [%d]\n", i, wpid, WTERMSIG(status));
            } else {
                printf("something different happened to the child\n");
            }
        }
    }
    return 0;
}
