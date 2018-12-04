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

int
main(int argc, char *argv[])
{
    int nproc, key, maxval;
    sscanf(argv[1], "%d", &nproc);
    sscanf(argv[2], "%d", &key);
    sscanf(argv[3], "%d", &maxval);
    if (nproc <= 0) {
        exit(0);
    }

    int semid = semget(key, nproc, 0660 | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        printf("error: %s\n", strerror(errno));
        exit(1);
    }
    set_sem(semid, nproc, 0);
    
    int *val = mmap(NULL, sizeof(maxval), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    semctl(semid, 0, SETVAL, 1);
    for (int i = 0; i < nproc; ++i) {
        int pid = fork();
        if (!pid) {
            while (4) {
                struct sembuf ops[NUM_OPS] = 
                {
                    { .sem_num = i, .sem_op = -1, .sem_flg = SEM_UNDO },
                };
                if (semop(semid, ops, NUM_OPS) < 0) {
                    printf("error: %s\n", strerror(errno));
                    exit(1);
                }
                if (*val > maxval) {
                    exit(0);
                }
                printf("%d: %d\n", i, (*val)++);
                fflush(stdout);
                if (*val > maxval) {
                    exit(0);
                }
                int x = (*val * *val) % nproc;
                ops[0].sem_num = x;
                ops[0].sem_op = 1;
                ops[0].sem_flg = SEM_UNDO;

                if (semop(semid, ops, NUM_OPS) < 0) {
                    printf("error: %s\n", strerror(errno));
                    exit(1);
                }
            }
        }
       
    }
    
    int status;
    wait(&status);
    set_sem(semid, nproc, 1);
    while (wait(NULL) > 0) {}
    semctl(semid, 0, IPC_RMID);
    munmap(val, sizeof(maxval));
    return 0;
}
