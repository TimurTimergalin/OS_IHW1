#ifndef MESSAGE_H
#define MESSAGE_H
#define buf_size 128

typedef struct {
    long mtype;
    char mtext[buf_size];
} message;

int try_create_q(int, int);
void try_delete_q(int);
void try_send(int, char *, int);
int try_receive(int, char *);
#endif //MESSAGE_H
