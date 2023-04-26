#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define FIFO_PATH "/tmp/my_fifo"
#define LIMIT 100

#define MY_RAND_MIN 0
#define MY_RAND_MAX 100

int generate_rng() {
    return rand() % MY_RAND_MAX + MY_RAND_MIN;
}

int main() {
    int fd_fifo;

//    unlink(FIFO_PATH);

    //if (mkfifo(FIFO_PATH, 0644) == -1) {
    //    perror("mkfifo");
    //    exit(EXIT_FAILURE);
    //}

    if ((fd_fifo = open(FIFO_PATH, O_APPEND | O_WRONLY)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    for (size_t i = 0; i < LIMIT; i++) {
        int rng = generate_rng();

        write(fd_fifo, &rng, sizeof(rng));
        printf("Сгенерировано число: %d\n", rng);
    }

    close(fd_fifo);

    return 0;
}
