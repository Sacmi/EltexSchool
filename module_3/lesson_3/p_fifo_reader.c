#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define FIFO_PATH "/tmp/my_fifo"
#define SEM_NAME "/my_sem"

int main() {
    int fd_fifo, buf;
    sem_t *sem;

    if ((fd_fifo = open(FIFO_PATH, O_RDONLY)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    sem = sem_open(SEM_NAME, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    while (read(fd_fifo, &buf, sizeof(buf)) > 0) {
        sem_wait(sem);
        printf("Прочитано число: %d\n", buf);
        sem_post(sem);
    }

    close(fd_fifo);
    sem_close(sem);
    unlink(FIFO_PATH);

    return 0;
}