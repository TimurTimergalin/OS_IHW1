#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>

#include "../common.h"
#include "message_queue.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Wrong args\n");
        exit(-1);
    }

    int key1 = atoi(argv[1]);
    int key2 = atoi(argv[2]);

    int qd1 = try_create_q(key1, IPC_CREAT | 0666);
    int qd2 = try_create_q(key2, IPC_CREAT | 0666);

    char buf[buf_size];

    int res = 0;
    for (int size = try_receive(qd1, buf);
         size != 0;
         size = try_receive(qd1, buf)) {
        res = validate_string(buf, size, res);
    }

    res = res == 0;

    char sres[2];
    int size = sprintf(sres, "%d", res);

    try_send(qd2, sres, size);
    return 0;
}
