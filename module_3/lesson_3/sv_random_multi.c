#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/sem.h>

#define READ 0
#define WRITE 1

#define MY_RAND_MIN 0
#define MY_RAND_MAX 100

#define TEMP_FILE "/tmp/random_result"
#define SEM_KEY 1234
#define FILE_SEM_KEY 5678
#define MAX_READERS 10

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "USAGE: times\n");
        exit(1);
    }

    size_t times = strtoul(argv[1], NULL, 10);
    int buf, ffd, pipefd[2];

    if (pipe(pipefd) != 0) {
        perror("pipe");
        exit(1);
    }

    int fd = creat(TEMP_FILE, 0666);
    if (fd == -1) {
        perror("creat");
        exit(1);
    } else {
        close(fd);
    }

    int sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
        exit(1);
    }

    union semun arg;
    arg.val = 0;
    if (semctl(sem_id, 0, SETVAL, arg) == -1) {
        perror("semctl");
        exit(1);
    }

    int file_sem_id = semget(FILE_SEM_KEY, 1, IPC_CREAT | 0666);
    if (file_sem_id == -1) {
        perror("semget");
        exit(1);
    }

    arg.val = MAX_READERS;
    if (semctl(file_sem_id, 0, SETVAL, arg) == -1) {
        perror("semctl");
        exit(1);
    }

    struct sembuf wait_op = {0, -1, SEM_UNDO};
    struct sembuf post_op = {0, 1, SEM_UNDO};

    switch (fork()) {
        case -1:
            perror("fork");
            exit(1);
        case 0:
            srand(time(NULL));
            close(pipefd[READ]);

            for (size_t i = 0; i < times; ++i) {
                int rng = rand() % MY_RAND_MAX + MY_RAND_MIN;

                printf("[CHILD] Записываю %d в канал\n", rng);
                if (write(pipefd[WRITE], &rng, sizeof(rng)) == -1) {
                    perror("write");
                    exit(1);
                }

                if (semop(sem_id, &wait_op, 1) == -1) {
                    perror("semop");
                    exit(1);
                }

                if (semop(file_sem_id, &wait_op, 1) == -1) {
                    perror("semop");
                    exit(1);
                }

                ffd = open(TEMP_FILE, O_RDONLY);
                if (ffd == -1) {
                    perror("open");
                    exit(1);
                }

                char buf_s[10];

                while (read(ffd, buf_s, sizeof(buf_s)) > 0) {
                    printf("%s", buf_s);
                }
                close(ffd);
                if (semop(file_sem_id, &post_op, 1) == -1) {
                    perror("semop");
                    exit(1);
                }
            }

            close(pipefd[WRITE]);
            break;
        default:
            close(pipefd[WRITE]);

            for (size_t i = 0; i < times && read(pipefd[READ], &buf, sizeof(buf)) > 0; ++i) {
                printf("[PARENT] Получил число %d -> %d\n", buf, buf * 2);
                buf = buf * 2;

                for (int k = 0; k < MAX_READERS; k++) {
                    if (semop(file_sem_id, &wait_op, 1) == -1) {
                        perror("semop");
                        exit(1);
                    }
                }

                ffd = open(TEMP_FILE, O_APPEND | O_WRONLY);
                if (ffd == -1) {
                    perror("open");
                    exit(1);
                }

                char buf_s[10];
                sprintf(buf_s, "%d\n", buf);
                write(ffd, buf_s, sizeof(buf_s));

                for (int k = 0; k < MAX_READERS; k++) {
                    if (semop(file_sem_id, &post_op, 1) == -1) {
                        perror("semop");
                        exit(1);
                    }
                }

                if (semop(sem_id, &post_op, 1) == -1) {
                    perror("semop");
                    exit(1);
                }
            }

            close(pipefd[READ]);
            wait(NULL);

            printf("Результат работы программы записан в файл `%s`\n", TEMP_FILE);
            break;
    }
}