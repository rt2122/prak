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

enum 
{ 
    NUM_OPS = 1,
    MEM_SIZE = 2,
};

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
    set_sem(semid, nproc, 0);
    
    unsigned *val = mmap(NULL, sizeof(maxval) * MEM_SIZE, PROT_READ | PROT_WRITE, 
            MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    semctl(semid, 0, SETVAL, 1);
    for (int i = 1; i <= nproc; ++i) {
        int pid = fork();
        if (!pid) {
            while (4) {
                struct sembuf ops[NUM_OPS] = 
                {
                    { .sem_num = i - 1, .sem_op = -1, .sem_flg = 0 },
                };
                if (semop(semid, ops, NUM_OPS) < 0) {
                    _exit(0);
                }

                printf("%d %d %d\n", i, (int)val[0], (int)val[1]);
                fflush(stdout);
                if (val[0] == maxval) {
                    _exit(0);
                }
                val[0]++;
                unsigned next = val[0] % nproc * val[0] % nproc * val[0] % nproc * val[0] % nproc + 1;
                
                ops[0].sem_num = (int)next - 1;
                ops[0].sem_op = 1;
                ops[0].sem_flg = 0;
                val[1] = i;

                semop(semid, ops, NUM_OPS);
            }
        }
       
    }
    
    wait(NULL);
    semctl(semid, 0, IPC_RMID);
    while (wait(NULL) > 0) {}
    return 0;
}
