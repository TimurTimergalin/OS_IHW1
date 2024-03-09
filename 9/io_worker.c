#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "../common.h"

const int buf_size = 128;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Wrong args\n");
        exit(-1);
    }

    char fifo1[] = "IHW1_8_FIFO1.fifo";
    char fifo2[] = "IHW1_8_FIFO2.fifo";

    mknod(fifo1, S_IFIFO | 0666, 0);
    mknod(fifo2, S_IFIFO | 0666, 0);

    int fifow = try_open(fifo1, O_WRONLY, -1);
    int readfiledes = try_open(argv[1], O_RDONLY, -1);

    char buf[buf_size];

    for (int size = read(readfiledes, buf, buf_size);
         size != 0;
         size = read(readfiledes, buf, buf_size)) {
        write_all(fifow, buf, size);
    }
    try_close(fifow);
    try_close(readfiledes);

    int fifor = try_open(fifo2, O_RDONLY, -1);
    char sres[1];
    read_all(fifor, sres, 1);
    int writefiledes = try_open(argv[2], O_WRONLY | O_CREAT, 0666);
    write_all(writefiledes, sres, 1);
    try_close(writefiledes);
    try_close(fifor);
    return 0;


}
