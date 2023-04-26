#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <semaphore.h>

#define FIFO_PATH "/tmp/my_fifo"
#define SEM_NAME "/my_sem"
#define LIMIT 10

#define MY_RAND_MIN 0
#define MY_RAND_MAX 100

int generate_rng() {
    return rand() % MY_RAND_MAX + MY_RAND_MIN;
}

int main() {
    int fd_fifo;
    sem_t *sem;

    sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    if ((fd_fifo = open(FIFO_PATH, O_WRONLY)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    for (size_t i = 0; i < LIMIT; i++) {
        int rng = generate_rng();

        sem_wait(sem);
        write(fd_fifo, &rng, sizeof(rng));
        sem_post(sem);

        printf("Сгенерировано число: %d\n", rng);
    }

    close(fd_fifo);
    sem_close(sem);

    return 0;
}