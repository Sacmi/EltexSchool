#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>

#define READ 0
#define WRITE 1

#define MY_RAND_MIN 0
#define MY_RAND_MAX 100

#define TEMP_FILE "/tmp/random_result"
#define SEM_NAME "/my_sem"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "USAGE: times\n");
        exit(EXIT_FAILURE);
    }

    size_t times = strtoul(argv[1], NULL, 10);
//    size_t times = 3;
    int buf, ffd, pipefd[2];

    if (pipe(pipefd) != 0) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    int fd = creat(TEMP_FILE, 0666);
    if (fd == -1) {
        perror("creat");
        exit(EXIT_FAILURE);
    } else {
        close(fd);
    }

    sem_t *sem;
    if ((sem = sem_open(SEM_NAME, O_CREAT, 0644, 0)) == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    switch (fork()) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            srand(time(NULL));
            close(pipefd[READ]);

            for (size_t i = 0; i < times; ++i) {
                int rng = rand() % MY_RAND_MAX + MY_RAND_MIN;

                printf("[CHILD] Записываю %d в канал\n", rng);
                if (write(pipefd[WRITE], &rng, sizeof(rng)) == -1) {
                    perror("write");
                    exit(EXIT_FAILURE);
                }

                sem_wait(sem);

                ffd = open(TEMP_FILE, O_RDONLY);
                if (ffd == -1) {
                    perror("open");
                    exit(EXIT_FAILURE);
                }

                char buf_s[10];

                while (read(ffd, buf_s, sizeof(buf_s)) > 0) {
                    printf("%s", buf_s);
                }

                close(ffd);
            }

            close(pipefd[WRITE]);
            break;
        default:
            close(pipefd[WRITE]);

            for (size_t i = 0; i < times && read(pipefd[READ], &buf, sizeof(buf)) > 0; ++i) {
                printf("[PARENT] Получил число %d -> %d\n", buf, buf * 2);
                buf = buf * 2;

                ffd = open(TEMP_FILE, O_APPEND | O_WRONLY);
                if (ffd == -1) {
                    perror("open");
                    exit(EXIT_FAILURE);
                }

                char buf_s[10];
                sprintf(buf_s, "%d\n", buf);
                write(ffd, buf_s, sizeof(buf_s));

                sem_post(sem);
            }

            close(pipefd[READ]);
            wait(NULL);

            printf("Результат работы программы записан в файл `%s`\n", TEMP_FILE);
            break;
    }
}