/*
 * Dieses Programm erzeugt zwei Subprozesse in denen jeweils ein Programm
 * aufgerufen wird.
 * Nach dem Start der beiden Subprozesse wartet der Elternprozess bis beide
 * Kindprozesse beendet wurden und beendet sich dann selbst.
 *
 * Aufrufsyntax:
 * factivate <PROGRAM 1> [<PARAM> ...] ; <PROGRAM 2> [<PARAM> ...]
 *
 * Beispielaufruf:
 * factivate ls -l -a /tmp \; sleep 5
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

#define MAX_ARGS 1024

/* Function to split the commands into tokens */
void parse_command(int start, int end, char **argv, char **cmd) {
    int j = 0;
    for (int i = start; i < end; i++) {
        cmd[j++] = argv[i];
    }
    cmd[j] = NULL;
}


int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s \"PROGRAMM 1 [args ...] ; PROGRAMM 2 [args ...]\"\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Find the delimiter index
    int delimiter_index = -1;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], ";") == 0) {
            delimiter_index = i;
            break;
        }
    }

    if (delimiter_index == -1) {
        fprintf(stderr, "Error: missing delimiter.\n");
        exit(EXIT_FAILURE);
    }

    // Parse Commands and Arguments
    char *cmd1[MAX_ARGS];
    char *cmd2[MAX_ARGS];

    parse_command(1, delimiter_index, argv, cmd1);
    parse_command(delimiter_index + 1, argc, argv, cmd2);

    pid_t pid1;
    if ((pid1 = fork()) == -1) {
        /* error */
        perror("Fork failed: ");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        /* child 1 */
        /* exec programm 1 */
        execvp(cmd1[0], cmd1);
        perror("exec1");
        exit(EXIT_FAILURE);
    }
    pid_t pid2;
    if ((pid2 = fork()) == -1) {
        perror("Fork failed ");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
        /* child 2 */
        /* exec program 2 */
        execvp(cmd2[0], cmd2);
        perror("exec2");
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid1, &status, 0);
        waitpid(pid2, &status, 0);
        
        exit(EXIT_SUCCESS);
    }
    return 0;
}
