#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

#define Q_KEY 1337
#define MSG_SIZE 100
#define MAX_CLIENTS 64

struct msgbuf {
    int mtype;
    char mtext[MSG_SIZE];
};

int main() {
    key_t server_key = Q_KEY;

    int msqid_s;
    if ((msqid_s = msgget(server_key, 0666 | IPC_CREAT)) < 0) {
        perror("msgget");
        exit(1);
    }

    int clients[MAX_CLIENTS];

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        clients[i] = -1;
    }

    struct msgbuf msgbuf;
    printf("Сервер запущен\n");

    while (1) {
        if (msgrcv(msqid_s, &msgbuf, sizeof(msgbuf.mtext), 0, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }

        printf("Получено сообщение от клиента %d\n", msgbuf.mtype);
        if (clients[msgbuf.mtype] < 0) {
            printf("Клиент только подключился. Открываю очередь\n");
            int msqid = msgget(msgbuf.mtype, 0666);
            if (msqid < 0) {
                fprintf(stderr, "Не удалось открыть очередь. Пропускаю сообщение\n");
                perror("mssget");
                continue;
            }

            clients[msgbuf.mtype] = msqid;
        }

        struct msgbuf sendbuf;
        sendbuf.mtype = msgbuf.mtype;
        char *str = calloc(96, sizeof(char));
        strcpy(str, msgbuf.mtext);
        snprintf(sendbuf.mtext, sizeof(sendbuf.mtext), "%d: %s", msgbuf.mtype, str);

        printf("Рассылка сообщения:\n");
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] < 0)
                continue;

            if ((msgsnd(clients[i], &sendbuf, sizeof(sendbuf.mtext), IPC_NOWAIT)) < 0) {
                fprintf(stderr, "Не удалось отправить сообщение клиенту %d\n", i);
                perror("msgsnd");
                exit(1);
            }

            printf(" - %d - отправлено (%d)\n", i, clients[i]);
        }
    }
}