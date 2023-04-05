#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 10
#define MAX_LINE 1024
#define COMMAND_EXIT "exit"

int main() {
    char *args[MAX_ARGS];
    char line[MAX_LINE];

    while (1) {
        printf("myshell> ");
        fflush(stdout);

        fgets(line, MAX_LINE, stdin);
        line[strlen(line) - 1] = '\0';

        int i = 0;
        char *cmd = strtok(line, " ");
        args[i] = strtok(NULL, " ");
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }

        if (strcmp(cmd, COMMAND_EXIT) == 0) {
            exit(EXIT_SUCCESS);
        }

        if (fork() == 0) {
            execvp(cmd, args);
        } else {
            wait(NULL);
        }
    }
}