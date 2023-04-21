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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: sv_writer <times>");
        exit(EXIT_FAILURE);
    }

    size_t times = strtoul(argv[1], NULL, 10);
    srand(time(NULL));

    key_t key = KEY;

    int msqid;
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < times; i++) {
        msgbuf buf = {.mtype = MSG_TYPE};
        snprintf(buf.mtext, sizeof(buf.mtext), "%d", rand());

        if (msgsnd(msqid, &buf, sizeof(buf.mtext), IPC_NOWAIT) < 0) {
            fprintf(stderr, "Unable to send '%s'", buf.mtext);
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }

        printf("Sent: %s\n", buf.mtext);
        sleep(SLEEP_SECS);
    }

    msgbuf exit_msg = {.mtype = END_TYPE};
    if (msgsnd(msqid, &exit_msg, sizeof(exit_msg.mtext), IPC_NOWAIT) < 0) {
        fprintf(stderr, "unable to send end message");
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    return 0;
}