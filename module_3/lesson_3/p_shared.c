#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

#define SHM_SIZE 1024
#define SHM_NAME "/posix_shm"

int *shm;
int count = 0;

void sigint_handler(int sig) {
    (void) sig;
    printf("Количество обработанных наборов данных: %d\n", count);
    munmap(shm, SHM_SIZE);
    shm_unlink(SHM_NAME);
    exit(0);
}

int main() {
    signal(SIGINT, sigint_handler);

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SHM_SIZE);
    shm = (int*) mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    srand(time(NULL));

    while (1) {
        int n = rand() % 100 + 1;
        shm[0] = n;
        for (int i = 1; i <= n; i++) {
            shm[i] = rand() % 1000;
        }

        if (fork() == 0) {
            int min = shm[1];
            int max = shm[1];
            for (int i = 2; i <= n; i++) {
                if (shm[i] < min) min = shm[i];
                if (shm[i] > max) max = shm[i];
            }
            shm[n + 1] = min;
            shm[n + 2] = max;
            exit(0);
        } else {
            wait(NULL);
            wait(NULL);
            wait(NULL);
            printf("Минимальное число: %d\n", shm[n + 1]);
            printf("Максимальное число: %d\n", shm[n + 2]);
            count++;
        }
    }
}