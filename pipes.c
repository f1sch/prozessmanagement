/*
 * Dieses Programm dient zur Veranschaulichung von Interprozesskommunikation
 * mit pipes.
 * Der Elternprozess schreibt die Zahlen 0 bis 49 in die erstellte
 * pipe und der Kindprozess liest diese Zahlen aus der pipe.
 * Die Reihenfolge davon ist abhängig davon wie der Scheduler
 * den Prozessen CPU-Zeit zuteilt. 
 * Der Elternprozess beginnt immer als erstes.
 * */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1

int main() {
    pid_t pid;
    int pfd[2]; /* Pipe file descriptors */
    size_t num_read;
    char buf[1024] = {0};
    if (pipe(pfd) == -1) { /* Create the pipe */
        perror("Pipe failed: "); exit(1);
    }
    if ((pid = fork()) == -1) { /* fork error */
        perror("Fork failed "); exit(1);
    } else if (pid == 0) { /* fork child */
        close(pfd[WRITE_END]); /* Close the write end of pipe. Not needed in child */
        for (;;) {
            memset(buf, 0, sizeof(buf));
            if ((num_read=read(pfd[READ_END], buf, 1024)) == 1) { /* Read pipe input */
                exit(1);
            } else if (num_read == 0) {
                break;
            } else {
                printf("    child read: [%s]\n", buf);
            }
        }
        exit(0);
    } else { /* fork parent */
        close(pfd[READ_END]); /* Close the Read end of pipe. Not needed in parent */
        for (int i = 0; i < 50; i++) {
            sprintf(buf, "%i ", i);
            printf("parent write: [%s]\n", buf);
            if (write(pfd[WRITE_END], buf, strlen(buf)) != strlen(buf)) {
                perror("parent write error");
            }
        }
        if (close(pfd[WRITE_END]) == -1) { /* Child will see EOF */
            perror("parent close pipe");
        }
        wait(NULL);
    }
}
