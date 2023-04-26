#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define FIFO_PATH "/tmp/my_fifo"
#define SEM_KEY 1234

int main() {
    int fd_fifo, buf;
    int sem_id;
    struct sembuf sem_op;

    sem_id = semget(SEM_KEY, 1, 0);
    if (sem_id == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    if ((fd_fifo = open(FIFO_PATH, O_RDONLY)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while (read(fd_fifo, &buf, sizeof(buf)) > 0) {
        sem_op.sem_num = 0;
        sem_op.sem_op = -1;
        sem_op.sem_flg = 0;
        semop(sem_id, &sem_op, 1);

        printf("Прочитано число: %d\n", buf);

        sem_op.sem_num = 0;
        sem_op.sem_op = 1;
        sem_op.sem_flg = 0;
        semop(sem_id, &sem_op, 1);
    }

    close(fd_fifo);
    unlink(FIFO_PATH);

    return 0;
}