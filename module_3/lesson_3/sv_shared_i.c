#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

#define SHM_SIZE 1024

int shmid;
int *shm;
int count = 0;

void sigint_handler(int sig) {
    (void) sig;
    printf("Количество обработанных наборов данных: %d\n", count);
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    exit(0);
}

int main() {
    signal(SIGINT, sigint_handler);

    key_t key = ftok("shmfile", 65);
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    shm = (int*) shmat(shmid, NULL, 0);

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
        } else if (fork() == 0) {
            int sum = 0;
            for (int i = 1; i <= n; i++) {
                sum += shm[i];
            }
            shm[n + 3] = sum;
            exit(0);
        } else if (fork() == 0) {
            double avg = 0.0;
            for (int i = 1; i <= n; i++) {
                avg += shm[i];
            }
            avg /= n;
            shm[n + 4] = avg;
            exit(0);
        } else {
            wait(NULL);
            wait(NULL);
            wait(NULL);
            printf("Минимальное число: %d\n", shm[n + 1]);
            printf("Максимальное число: %d\n", shm[n + 2]);
            printf("Сумма чисел: %d\n", shm[n + 3]);
            printf("Среднее значение чисел: %d\n", shm[n + 4]);
            count++;
        }
    }
}