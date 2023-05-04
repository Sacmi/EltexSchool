#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/sem.h>

#define FIFO_PATH "/tmp/my_fifo"
#define SEM_KEY 1234
#define LIMIT 5

#define MY_RAND_MIN 0
#define MY_RAND_MAX 100

int generate_rng() {
    return rand() % MY_RAND_MAX + MY_RAND_MIN;
}

int main() {
    int fd_fifo;
    struct sembuf sem_op;
    sem_op.sem_flg = 0;
    sem_op.sem_num = 0;

    int sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0644);
    if (sem_id == -1) {
        perror("semget");
        exit(1);
    }

    sem_op.sem_op = 1;
    semop(sem_id, &sem_op, 1);

    if ((fd_fifo = open(FIFO_PATH, O_WRONLY)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    for (size_t i = 0; i < LIMIT; i++) {
        int rng = generate_rng();

        sem_op.sem_op = -1;
        semop(sem_id, &sem_op, 1);
        write(fd_fifo, &rng, sizeof(rng));
        sem_op.sem_op = 1;
        semop(sem_id, &sem_op, 1);

        printf("Сгенерировано число: %d\n", rng);
    }

    close(fd_fifo);

    return 0;
}
