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

    int pipedes1[2];
    try_pipe(pipedes1);

    int chpid = try_fork();

    if (chpid > 0) {  // First process
        try_close(pipedes1[0]);
        int readfiledes = try_open(argv[1], O_RDONLY, -1);
        char buf[buf_size + 1];
        int size = read_max(readfiledes, buf, buf_size);
        write_all(pipedes1[1], buf, size);
        try_close(pipedes1[1]);
        try_close(readfiledes);
        return 0;
    }

    try_close(pipedes1[1]);

    int pipedes2[2];
    try_pipe(pipedes2);

    chpid = try_fork();

    if (chpid > 0) {  // Second process
        try_close(pipedes2[0]);
        char buf[buf_size];
        int size = read(pipedes1[0], buf, buf_size);
        int res = validate_string(buf, size, 0) == 0;
        char sres[2];
        size = sprintf(sres, "%d", res);
        write_all(pipedes2[1], sres, size);
        try_close(pipedes2[1]);
        return 0;
    }

    // Third process
    try_close(pipedes2[1]);
    char buf[1];
    read_all(pipedes2[0], buf, 1);
    try_close(pipedes2[0]);
    int writefiledes = try_open(argv[2], O_WRONLY | O_CREAT, 0666);
    write_all(writefiledes, buf, 1);
    try_close(writefiledes);
    return 0;
}
