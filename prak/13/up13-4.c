#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

enum 
{ 
    KEY = 4444,
    N_OPS = 1,
};

void
set_sem(int semid, int n, unsigned short val)
{
    for (int i = 0; i < n; ++i) {
        semctl(semid, i, SETVAL, val);
    }
}

int
main(int argc, char *argv[])
{
    int count;
    sscanf(argv[1], "%d", &count);
    int semid = semget(KEY, count, 0660 | IPC_CREAT | IPC_EXCL);
    set_sem(semid, count, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
    for (int i = 0; i < count; ++i) {
        if (!fork()) {
            while (4) {
                struct sembuf ops[N_OPS] =
                    {
                        { .sem_num = i, .sem_op = -1, .sem_flg = 0 },
                    };
                if (semop(semid, ops, N_OPS) < 0) {
                    _exit(0);
                }
                int val;
                if (scanf("%d", &val) != 1) {
                    _exit(0);
                }
                printf("%d %d\n", i, val);
                fflush(stdout);
                int next = val % count;
                if (next < 0) {
                    next += count;
                }
                ops[0].sem_num = next;
                ops[0].sem_op = 1;
                semop(semid, ops, N_OPS);
            }
        }
    }
    semctl(semid, 0, SETVAL, 1);
    wait(NULL);
    semctl(semid, 0, IPC_RMID);
    while (wait(NULL) > 0) {}
    _exit(0);
}
