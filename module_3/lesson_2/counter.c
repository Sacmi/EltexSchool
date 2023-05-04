#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FILE_PATH "/tmp/counter"

/* 
	При SIGINT процесс завершается и выдает Interrupt
	При SIGQUIT процесс завершается и выдает Quit
	При SIGABRT процесс завершается и выдает Aborted
	При SIGKILL процесс завершается и выдает Killed
	При SIGTERM процесс завершается и выдает Terminated
	При SIGTSTP процесс останавливается и выдает Stopped 
	При SIGSTOP процесс останавливается и выдает Stoppped
	При SIGCONT процесс восстанавливается
*/ 

int main() {
    size_t counter = 0;
    FILE *file = fopen(FILE_PATH, "w");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        fprintf(file, "%zu\n", counter);
        counter++;
        sleep(1);
    }
}
