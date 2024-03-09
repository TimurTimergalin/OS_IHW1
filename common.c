#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int validate_string(char *str, int n, int prev) {
    if (prev < 0) {
        return prev;
    }
    for (int i = 0; i < n; ++i) {
        char c = str[i];
        if (c == '(') {
            ++prev;
        } else if (c == ')') {
            --prev;
            if (prev < 0) {
                return prev;
            }
        }
    }
    return prev;
}

void try_pipe(int fd[]) {
    if (pipe(fd) < 0) {
        printf("Cannot pipe\n");
        exit(-1);
    }
}

int try_open(char *name, int mode, int acc) {
    int res;

    if (acc != -1) {
        res = open(name, mode, acc);
    } else {
        res = open(name, mode);
    }

    if (res < 0) {
        printf("Cannot open file\n");
        exit(-1);
    }
    return res;
}

void try_close(int fd) {
    if (close(fd) < 0) {
        printf("Cannot close descriptor\n");
        exit(-1);
    }
}

int try_fork() {
    int res = fork();
    if (res < 0) {
        printf("Cannot fork\n");
    }
    return res;
}

int read_max(int fd, char *buf, int len) {
    int res;
    if ((res = read(fd, buf, len + 1)) > len) {
        printf("File is too large\n");
        exit(-1);
    }
    return res;
}

void read_all(int fd, char *buf, int len) {
    if (read(fd, buf, len) != len) {
        printf("Unable to read all at once\n");
        exit(-1);
    }
}

void write_all(int fd, char *buf, int  len) {
    if (write(fd, buf, len) != len) {
        printf("Unable to write all at once\n");
        exit(-1);
    }
}
