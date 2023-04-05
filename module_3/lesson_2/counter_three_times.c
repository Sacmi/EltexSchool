#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define FILE_PATH "/tmp/counter"

size_t current_state = 0;

void listener(int sig) {
    (void) sig;

    if (++current_state == 3) {
        printf("Третий сигнал. Выход.\n");
        exit(EXIT_SUCCESS);
    } else {
        printf("Получил сигнал\n");
    }
}

int main() {
    signal(SIGINT, listener);

    size_t counter = 0;
    FILE *file = fopen(FILE_PATH, "w");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        fprintf(file, "%zu\n", ++counter);
        sleep(1);
    }
}