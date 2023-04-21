#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

#define MSG_SIZE 100

struct msgbuf {
    int mtype;
    char mtext[MSG_SIZE];
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: sv_chat <sender> <receiver>\n");
        exit(1);
    }

    key_t sender, receiver;
    sscanf(argv[1], "%d", &sender);
    sscanf(argv[2], "%d", &receiver);

    int msqid_s, msquid_r;
    if ((msqid_s = msgget(sender, 0666 | IPC_CREAT)) < 0) {
        perror("msgget");
        exit(1);
    }

    if ((msquid_r = msgget(receiver, 0666 | IPC_CREAT)) < 0) {
        perror("msgget");
        exit(1);
    }

    printf("Очередь для отправки: %d, для получения %d\n", msqid_s, msquid_r);

    struct msgbuf msgbuf;
    msgbuf.mtype = 1;

    switch (fork()) {
        case -1:
            perror("fork");
            exit(1);
        case 0:
            while (1) {
                if (msgrcv(msquid_r, &msgbuf, sizeof(msgbuf.mtext), 0, 0) < 0) {
                    perror("msgrcv");
                    exit(1);
                }

                fprintf(stdout, "< %s\n", msgbuf.mtext);
            }
        default:
            while (1) {
                fgets(msgbuf.mtext, MSG_SIZE, stdin);
                msgbuf.mtext[strcspn(msgbuf.mtext, "\n")] = '\0';

                if (msgsnd(msqid_s, &msgbuf, sizeof(msgbuf.mtext), IPC_NOWAIT) < 0) {
                    perror("msgsnd");
                    exit(1);
                }
            }
    }
}