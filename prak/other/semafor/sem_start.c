#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

enum { NUM_OPS = 1 };

void set_sem(int semid, int nsem, int val)
{
    for (int i = 0; i < nsem; ++i) {
        semctl(semid, i, SETVAL, val);
    }
}

void
set_sembuf(struct sembuf *ops, int n, int *num, int op, int flg)
{
    for (int i = 0; i < n; ++i) {
        ops[i].sem_num = num[i];
        ops[i].sem_op = op;
        ops[i].sem_flg = flg;
    }
}

int
main(int argc, char *argv[])
{
    

    int semid = semget(key, nproc, 0660 | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        printf("error: %s\n", strerror(errno));
        exit(1);
    }
    int *val = mmap(NULL, sizeof(maxval), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    semctl(semid, 0, IPC_RMID);
    munmap(val, sizeof(maxval));
    return 0;
}
