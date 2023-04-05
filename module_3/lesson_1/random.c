#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1

#define MY_RAND_MIN 0
#define MY_RAND_MAX 100

#define TEMP_FILE "/tmp/random_result"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "USAGE: times\n");
        exit(EXIT_FAILURE);
    }

    size_t times = strtoul(argv[1], NULL, 10);
    int fd[2];
    int another_fd[2];
    int buf;

    if (pipe(fd) != 0 || pipe(another_fd) != 0) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    switch (fork()) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            srand(time(NULL));
            close(fd[READ]);
            close(another_fd[WRITE]);

            for (size_t i = 0; i < times; ++i) {
                int rng = rand() % MY_RAND_MAX + MY_RAND_MIN;

                printf("[CHILD] Записываю %d в канал\n", rng);
                if (write(fd[WRITE], &rng, sizeof(rng)) == -1) {
                    perror("write");
                    exit(EXIT_FAILURE);
                }
            }

            close(fd[WRITE]);



            for (size_t i = 0; i < times; ++i) {
                if (read(another_fd[READ], &buf, sizeof(buf)) <= 0) {
                    perror("read");
                    exit(EXIT_FAILURE);
                }
                printf("[CHILD] Получил число %d\n", buf);
            }

            close(another_fd[READ]);
            break;
        default:
            close(fd[WRITE]);
            close(another_fd[READ]);

            FILE *file = fopen(TEMP_FILE, "w");
            if (file == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
            }

            for (size_t i = 0; i < times && read(fd[READ], &buf, sizeof(buf)) > 0; ++i) {
                printf("[PARENT] Получил число %d -> %d\n", buf, buf * 2);
                buf = buf * 2;
                fprintf(file, "%d\n", buf);
                write(another_fd[WRITE], &buf, sizeof(buf));
            }

            close(fd[READ]);
            fclose(file);

            wait(NULL);

            printf("Результат работы программы записан в файл `%s`\n", TEMP_FILE);
            break;
    }
}