/*
 * Dieses Programm simuliert die Ein-/Ausgabeumlenkung unter Linux-Systemen.
 * Es ruft ein vom Benutzer angegebenes Programm auf und lenkt dessen 
 * Standardausgabe und Standard-Fehlerausgabe in eine angegebene Dateien um.
 * 
 * Aufrufsyntax:
 * redirect <OUTPUT> <ERROR> <PROGRAM> [<PARAM> ...]
 * 
 * <OUTPUT> und <ERROR> können wahlweise durch "-" ersetzt werden, wenn
 * diese Umlenkung nicht stattfinden soll.
 *
 * Beispielaufrufe:
 * redirect /tmp/test1.out - ps -aux
 * redirect /tmp/test2.out /tmp/test2.err cp /etc/hosts /etc/foo /tmp
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void parse_command(int end, char **argv, char **cmd) {
    int start = 3;
    int j = 0;
    for (int i = start; i < end; i++) {
        cmd[j++] = argv[i];
    }
    cmd[j] = NULL;
}

void redirect_output(const char *output, const char *error) {
    int fd_out, fd_err;

    // Redirect standard output if output is not "-"
    if (strcmp(output, "-") != 0) {
        fd_out = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_out == -1) {
            perror("open output file");
            exit(EXIT_FAILURE);
        }
        if (dup2(fd_out, STDOUT_FILENO) == -1) {
            perror("dup2 output file");
            exit(EXIT_FAILURE);
        }
        close(fd_out);
    }
    // Redirect standard output if output is not "-"
    if (strcmp(error, "-") != 0) {
        fd_err = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_err == -1) {
            perror("open output file");
            exit(EXIT_FAILURE);
        }
        if (dup2(fd_err, STDOUT_FILENO) == -1) {
            perror("dup2 output file");
            exit(EXIT_FAILURE);
        }
        close(fd_err);
    }

}

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <OUTPUT> <ERROR> <PROGRAM> [<PARAM> ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    const char *output = argv[1];
    const char *error = argv[2];
    char **program_args = &argv[3];

    // Redirect the output and error streams
    redirect_output(output, error);

    // Execute the program with given parameters
    execvp(program_args[0], program_args);

    // If execvp returns, an error occured
    perror("execvp");
    exit(EXIT_FAILURE);
}
