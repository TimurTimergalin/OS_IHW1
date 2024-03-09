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

    char fifo1[] = "IHW1_7_FIFO1.fifo";
    char fifo2[] = "IHW1_7_FIFO2.fifo";

    mknod(fifo1, S_IFIFO | 0666, 0);
    mknod(fifo2, S_IFIFO | 0666, 0);

    int chpid = try_fork();

    if (chpid > 0) { // Parent
        int fifow = try_open(fifo1, O_WRONLY, -1);

        int readfiledes = try_open(argv[1], O_RDONLY, -1);
        char buf[buf_size + 1];
        int size = read_max(readfiledes, buf, buf_size);
        try_close(readfiledes);

        write_all(fifow, buf, size);
        try_close(fifow);

        int fifor = try_open(fifo2, O_RDONLY, -1);
        char sres[1];
        read_all(fifor, sres, 1);
        int writefiledes = try_open(argv[2], O_WRONLY | O_CREAT, 0666);
        write_all(writefiledes, sres, 1);
        try_close(writefiledes);
        try_close(fifor);
    } else {  // Child
        int fifor = try_open(fifo1, O_RDONLY, -1);
        char buf[buf_size];
        int size = read(fifor, buf, buf_size);
        int res = validate_string(buf, size, 0) == 0;
        char sres[2];
        size = sprintf(sres, "%d", res);

        int fifow = try_open(fifo2, O_WRONLY, -1);
        write_all(fifow, sres, size);
        try_close(fifor);
        try_close(fifow);
    }

    return 0;
}
