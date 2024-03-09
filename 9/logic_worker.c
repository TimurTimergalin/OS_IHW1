#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "../common.h"

const int buf_size = 128;

int main() {
    char fifo1[] = "IHW1_8_FIFO1.fifo";
    char fifo2[] = "IHW1_8_FIFO2.fifo";

    int fifor = try_open(fifo1, O_RDONLY, -1);
    char buf[buf_size];

    int res = 0;
    for (int size = read(fifor, buf, buf_size);
         size != 0;
         size = read(fifor, buf, buf_size)) {
        res = validate_string(buf, size, res);
    }
    try_close(fifor);

    res = res == 0;

    char sres[2];
    int size = sprintf(sres, "%d", res);

    int fifow = try_open(fifo2, O_WRONLY, -1);
    write_all(fifow, sres, size);
    try_close(fifow);
    return 0;
}