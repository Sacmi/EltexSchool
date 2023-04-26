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
#define FILE_SEM_NAME "/my_file_sem"
#define MAX_READERS 10

/*
 * Например, вы можете инициализировать семафор с начальным значением равным максимальному количеству процессов, которые могут читать файл одновременно.
 * Когда процесс хочет прочитать файл, он должен выполнить операцию sem_wait для этого семафора. Когда процесс заканчивает чтение файла, он должен выполнить операцию sem_post для этого семафора. Когда процесс хочет записать в файл, он должен выполнить операцию sem_wait столько раз, сколько равно максимальному количеству процессов, которые могут читать файл одновременно. Когда процесс заканчивает запись в файл, он должен выполнить операцию sem_post столько же раз.
 */

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

    sem_t *sem, *file_sem;
    if ((sem = sem_open(SEM_NAME, O_CREAT, 0644, 0)) == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    if ((file_sem = sem_open(FILE_SEM_NAME, O_CREAT, 0644, MAX_READERS)) == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

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

                sem_wait(sem);
                sem_wait(file_sem);

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
                sem_post(file_sem);
            }

            close(pipefd[WRITE]);
            break;
        default:
            close(pipefd[WRITE]);

            for (size_t i = 0; i < times && read(pipefd[READ], &buf, sizeof(buf)) > 0; ++i) {
                printf("[PARENT] Получил число %d -> %d\n", buf, buf * 2);
                buf = buf * 2;

                for (int k = 0; k < MAX_READERS; k++) {
                    sem_wait(file_sem);
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
                    sem_post(file_sem);
                }
                sem_post(sem);
            }

            close(pipefd[READ]);
            wait(NULL);

            printf("Результат работы программы записан в файл `%s`\n", TEMP_FILE);
            break;
    }
}