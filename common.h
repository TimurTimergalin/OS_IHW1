#ifndef COMMON_H
#define COMMON_H

int validate_string(char *, int, int);

void try_pipe(int[]);

int try_open(char *, int, int);

void try_close(int);

int try_fork();

int read_max(int, char*, int);

void read_all(int, char*, int);

void write_all(int, char*, int);
#endif //COMMON_H
