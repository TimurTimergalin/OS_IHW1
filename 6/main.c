#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "../common.h"

const int buf_size = 5000;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Wrong args\n");
        exit(-1);
    }

    int fd1[2];
    int fd2[2];

    try_pipe(fd1);
    try_pipe(fd2);

    int chpid = try_fork();

    if (chpid > 0) {  // Parent
        try_close(fd1[0]);
        try_close(fd2[1]);

        int readfiledes = try_open(argv[1], O_RDONLY, -1);
        char buf[buf_size + 1];
        int size = read_max(readfiledes, buf, buf_size);
        try_close(readfiledes);

        write_all(fd1[1], buf, size);
        try_close(fd1[1]);

        char sres[1];
        read_all(fd2[0], sres, 1);

        int writefiledes = try_open(argv[2], O_WRONLY | O_CREAT, 0666);
        write_all(writefiledes, sres, 1);
        try_close(writefiledes);
        try_close(fd2[0]);
    } else {  // Child
        try_close(fd1[1]);
        try_close(fd2[0]);

        char buf[buf_size];
        int size = read(fd1[0], buf, buf_size);
        try_close(fd1[0]);
        int res = validate_string(buf, size, 0) == 0;
        char sres[2];
        size = sprintf(sres, "%d", res);

        write_all(fd2[1], sres, size);
        try_close(fd2[1]);
    }

    return 0;
}