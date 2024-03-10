#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "message_queue.h"


int try_create_q(int key, int mode) {
    int res;
    if ((res = msgget(key, mode)) < 0) {
        printf("Cannot create a message queue\n");
        exit(-1);
    }
    return res;
}

void try_delete_q(int qd) {
    if (msgctl(qd, IPC_RMID, NULL) < 0) {
        printf("Cannot delete queue\n");
    }
}

void try_send(int qd, char *msg, int size) {
    message m;
    strncpy(m.mtext, msg, buf_size);
    m.mtype = 1;

    if (msgsnd(qd, &m, size, 0) < 0) {
        printf("Cannot put message into queue\n");
        exit(-1);
    }
}

int try_receive(int qd, char *dest) {
    message m;
    int size;
    if ((size = msgrcv(qd, &m, buf_size, 0, 0)) < 0) {
        printf("Cannot read message from queue\n");
        exit(-1);
    }
    strncpy(dest, m.mtext, size);
    return size;
}
