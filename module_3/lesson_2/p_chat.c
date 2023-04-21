#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>

#define MSG_SIZE 100

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: p_chat <sender> <receiver>\n");
        exit(1);
    }

    char *sender = argv[1];
    char *receiver = argv[2];

    mqd_t ds_s, ds_r;
    struct mq_attr attr_s;
    attr_s.mq_flags = 0;
    attr_s.mq_maxmsg = 6;
    attr_s.mq_msgsize = MSG_SIZE;
    attr_s.mq_curmsgs = 0;

    printf("Открытие очереди для отправки\n");
    if ((ds_s = mq_open(sender, O_CREAT | O_WRONLY | O_NONBLOCK, 0666, &attr_s)) == -1) {
        perror("mq_open");
        exit(1);
    }

    printf("Открытие очереди для получения\n");
    if ((ds_r = mq_open(receiver, O_CREAT | O_RDONLY, 0666, &attr_s)) == -1) {
        perror("mq_open");
        exit(1);
    }

    char message[MSG_SIZE];

    switch (fork()) {
        case -1:
            perror("fork");
            exit(1);
        case 0:
            while (1) {
                if (mq_receive(ds_r, message, MSG_SIZE, NULL) == -1) {
                    perror("mq_receive");
                    exit(1);
                }

                printf("%s\n", message);
                message[0] = '\0';
            }
        default:
            while (1) {
                fgets(message, MSG_SIZE, stdin);
                message[strcspn(message, "\n")] = '\0';

                if (mq_send(ds_s, message, strlen(message), 1) == -1) {
                    perror("mq_send");
                    exit(1);
                }
            }
    }
}