#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define FILE_PATH "/tmp/counter"

int main() {
    signal(SIGINT, SIG_IGN);

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