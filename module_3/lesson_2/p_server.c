#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Q_SERVER "/server"
#define MSG_SIZE 256
#define MAX_CLIENTS 32

int main() {
    mqd_t ds;
    struct mq_attr attr_s;
    attr_s.mq_flags = 0;
    attr_s.mq_maxmsg = 6;
    attr_s.mq_msgsize = MSG_SIZE;
    attr_s.mq_curmsgs = 0;

    mqd_t clients[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        clients[i] = -1;
    }

    if ((ds = mq_open(Q_SERVER, O_CREAT | O_RDONLY, 0666, &attr_s)) == -1) {
        perror("mq_open");
        exit(1);
    }

    char buf[MSG_SIZE];
    unsigned int prio;

    while (1) {
        if (mq_receive(ds, buf, MSG_SIZE, &prio) == -1) {
            fprintf(stderr, "Не получилось получить сообщение. Пропуск\n");
            continue;
        }

        if (clients[prio] < 0) {
            printf("Первое сообщение от клиента. Создание ответной очереди\n");
            char client_name[32];
            snprintf(client_name, 32, "/client_%d", prio);

            mqd_t ds_c;
            if ((ds_c = mq_open(client_name, O_WRONLY, 0666, &attr_s)) == -1) {
                perror("mq_open");
                exit(1);
            }

            clients[prio] = ds_c;
        }

        printf("Получено сообщение от клиента %d\n", prio);

        for (int i = 0; i < MAX_CLIENTS; ++i) {
            mqd_t client = clients[i];

            if (client < 0 || i == (int)prio) {
                continue;
            }

            if (mq_send(client, buf, strlen(buf), 1) == -1) {
                perror("mq_send");
                exit(1);
            }
        }

        buf[0] = '\0';
    }
}