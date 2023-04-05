#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void on_exit_handler(int status, void *arg) {
    (void) arg;
    printf("Статус: %d\n", status);
}

int main(int argc, char *argv[]) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        printf("Child:\n");
    } else {
        printf("Parent:\n");
    }

    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }

    on_exit(on_exit_handler, NULL);

    return 0;
}