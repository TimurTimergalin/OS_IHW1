#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>

#include "../common.h"
#include "message_queue.h"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Wrong args\n");
        exit(-1);
    }

    int key1 = atoi(argv[3]);
    int key2 = atoi(argv[4]);

    int qd1 = try_create_q(key1, IPC_CREAT | 0666);
    int qd2 = try_create_q(key2, IPC_CREAT | 0666);

    int readfiledes = try_open(argv[1], O_RDONLY, -1);

    char buf[buf_size];

    for (int size = read(readfiledes, buf, buf_size);
         size != 0;
         size = read(readfiledes, buf, buf_size)) {
        try_send(qd1, buf, size);
    }
    try_send(qd1, buf, 0);
    try_close(readfiledes);

    char sres[1];
    (void) try_receive(qd2, sres);
    int writefiledes = try_open(argv[2], O_WRONLY | O_CREAT, 0666);
    write_all(writefiledes, sres, 1);
    try_close(writefiledes);
    try_delete_q(qd1);
    try_delete_q(qd2);
    return 0;
}
