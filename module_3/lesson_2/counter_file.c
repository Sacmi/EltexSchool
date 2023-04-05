#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>

#define FILE_PATH "/tmp/counter"

size_t current_state = 0;

void listener(int sig) {
    switch (sig) {
        case SIGINT:
            printf("Полуичл SIGINT\n");
            break;
        case SIGQUIT:
            printf("Получил SIGQUIT\n");
            break;
        default:
            assert(0 && "unreachable");
    }
}

int main() {
    signal(SIGINT, listener);
    signal(SIGQUIT, listener);

    size_t counter = 0;
    FILE *file = fopen(FILE_PATH, "w");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);

        fprintf(file, "%zu\n", ++counter);

        signal(SIGINT, listener);
        signal(SIGQUIT, listener);
        sleep(1);
    }
}