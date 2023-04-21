#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>

#define KEY 777
#define SLEEP_SECS 1
#define MSG_TYPE 1
#define END_TYPE 255
#define TEXT_SIZE 32

typedef struct msgbuf {
    long mtype;
    char mtext[TEXT_SIZE];
} msgbuf;

int main() {
    key_t key = KEY;

    int msqid;
    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    while (1) {
        msgbuf buf;

        if (msgrcv(msqid, &buf, sizeof(buf.mtext), 0L, IPC_NOWAIT) < 0) {
            perror("msgrcv");
            msgctl(msqid, IPC_RMID, NULL);
            exit(EXIT_FAILURE);
        }

        if (buf.mtype == END_TYPE) {
            printf("Получил сигнал для выхода\n");
            return 0;
        }

        printf("Получил сообщение: %s\n", buf.mtext);
    }
}