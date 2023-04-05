#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 10
#define MAX_LINE 1024
#define MAX_COMMANDS 10

#define COMMAND_EXIT "exit"
#define PIPE_DELIM "|"

void close_pipes(int pipes[][2], int num_pipes) {
    for (int i = 0; i < num_pipes; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
}

int main() {
    char line[MAX_LINE];
    char *commands[MAX_COMMANDS];
    int num_commands;
    int pipes[MAX_COMMANDS - 1][2];
    int i;

    while (1) {
        printf("pipeshell> ");
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }

        // Разбиваем строку на команды
        commands[0] = strtok(line, PIPE_DELIM);
        num_commands = 1;
        while ((commands[num_commands] = strtok(NULL, PIPE_DELIM)) != NULL) {
            num_commands++;
        }

        // Создаем каналы для конвейера
        for (i = 0; i < num_commands - 1; i++) {
            pipe(pipes[i]);
        }

        // Запускаем команды
        for (i = 0; i < num_commands; i++) {
            char *args[MAX_ARGS];
            int num_args;

            // Разбиваем команду на аргументы
            args[0] = strtok(commands[i], " \t\n");
            num_args = 1;
            while ((args[num_args] = strtok(NULL, " \t\n")) != NULL) {
                num_args++;
            }

            if (strcmp(args[0], COMMAND_EXIT) == 0) {
                close_pipes(pipes, num_commands - 1);
                exit(EXIT_SUCCESS);
            }

            if (fork() == 0) {
                // Перенаправляем ввод/вывод для конвейера
                if (i > 0) {
                    dup2(pipes[i - 1][0], STDIN_FILENO);
                }
                if (i < num_commands - 1) {
                    dup2(pipes[i][1], STDOUT_FILENO);
                }

                // Закрываем все каналы
                for (i = 0; i < num_commands - 1; i++) {
                    close(pipes[i][0]);
                    close(pipes[i][1]);
                }

                execvp(args[0], args);
                perror(args[0]);
                exit(1);
            }
        }

        // Закрываем все каналы в родительском процессе
        close_pipes(pipes, num_commands - 1);

        // Ждем завершения всех дочерних процессов
        for (i = 0; i < num_commands; i++) {
            wait(NULL);
        }
    }

    return 0;
}