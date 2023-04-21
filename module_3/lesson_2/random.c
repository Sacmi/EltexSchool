#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <assert.h>
#include <fcntl.h>

#define READ 0
#define WRITE 1

#define MY_RAND_MIN 0
#define MY_RAND_MAX 100

#define TEMP_FILE "/tmp/random_result"
#define TIME_TO_SLEEP 1

int is_locked = 1;
int aye = 0;

void child_listener(int sig) {
    switch (sig) {
        case SIGUSR1:
            is_locked = 1;
            break;
        case SIGUSR2:
            is_locked = 0;
            break;
        default:
            assert(0 && "unreachable");
    }

    aye = 1;
}

void waiting() {
    while (!aye) {
        sleep(TIME_TO_SLEEP);
    }

    aye = 0;
}

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

    pid_t pid;

    switch (pid = fork()) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            srand(time(NULL));
            close(pipefd[READ]);

            signal(SIGUSR1, child_listener);
            signal(SIGUSR2, child_listener);

            for (size_t i = 0; i < times; ++i) {
                int rng = rand() % MY_RAND_MAX + MY_RAND_MIN;

                printf("[CHILD] Записываю %d в канал\n", rng);
                if (write(pipefd[WRITE], &rng, sizeof(rng)) == -1) {
                    perror("write");
                    exit(EXIT_FAILURE);
                }

                while (is_locked) {
                    waiting();
                }

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

                /*
                 * По условию задания, дочерний процесс может отправить число после прочтения.
                 * Если строго следовать заданию, то строка кода ниже не нужна, но
                 * будет выводиться только первое число, так как дочерний процесс может быстрее
                 * считывать и генерировать числа, чем родительский процесс их обработать. (пример: `1`, `1`, `1`)
                 * Если дочерний процесс сам для себя блокирует,
                 * то будут выводиться числа последовательно (пример: `1`, `1 2`, `1 2 3`).
                 */
                is_locked = 1;
            }

            close(pipefd[WRITE]);
            break;
        default:
            close(pipefd[WRITE]);

            for (size_t i = 0; i < times && read(pipefd[READ], &buf, sizeof(buf)) > 0; ++i) {
                kill(pid, SIGUSR1);
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

                kill(pid, SIGUSR2);
            }

            close(pipefd[READ]);
            wait(NULL);

            printf("Результат работы программы записан в файл `%s`\n", TEMP_FILE);
            break;
    }
}