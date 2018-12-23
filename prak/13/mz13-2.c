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
#include <limits.h>

enum 
{ 
    N_OPS = 2,
    MAX_VAL = 10,
    SEED_SHIFT = 5    
};

void 
set_sem(int semid, int nsem, int val)
{
    for (int i = 0; i < nsem; ++i) {
        semctl(semid, i, SETVAL, val);
    }
}

int 
get_rnd(int max)
{
    double part = rand() / (RAND_MAX + 1.0);
    return (int) (part * max);
}
void
operation(int *data, int ind1, int ind2, int value)
{
    if (ind1 != ind2) {
        int tmp1 = data[ind1] - value;
        int tmp2 = data[ind2] + value;

        data[ind1] = tmp1;
        data[ind2] = tmp2;
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
    int count, key, nproc, iter_count;
    sscanf(argv[1], "%d", &count);
    sscanf(argv[2], "%d", &key);
    sscanf(argv[3], "%d", &nproc);
    sscanf(argv[4], "%d", &iter_count);
    if (nproc <= 0) {
        exit(0);
    }
    int *seed = mmap(NULL, sizeof(int) * nproc, PROT_READ|PROT_WRITE,
            MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    for (int i = 0; i < nproc; ++i) {
        sscanf(argv[i + SEED_SHIFT], "%d", &seed[i]);
    }

    int semid = semget(key, count, 0660 | IPC_CREAT | IPC_EXCL);
    set_sem(semid, count, 1);
    
    int *mem = mmap(NULL, sizeof(int) * count, PROT_READ | PROT_WRITE, 
            MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    for (int i = 0; i < count; ++i) {
        scanf("%d", &mem[i]);
    }

    for (int i = 0; i < nproc; ++i) {
        int pid = fork();
        if (!pid) {
            srand(seed[i]);
            for (int j = 0; j < iter_count; ++j) {
                int ind[N_OPS] = { get_rnd(count), get_rnd(count)}, val = get_rnd(MAX_VAL);
                int n = ind[0] != ind[1] ? N_OPS : 1;
                
                struct sembuf ops[n];
                set_sembuf(ops, n, ind, -1, 0);
                semop(semid, ops, n);
                operation(mem, ind[0], ind[n - 1], val);
                
                set_sembuf(ops, n, ind, 1, 0);
                semop(semid, ops, n);
            }
            exit(0);
        }
       
    }
    
    while (wait(NULL) > 0) {}
    semctl(semid, 0, IPC_RMID);
    for (int i = 0; i < count; ++i) {
        printf("%d ", mem[i]);
    }
    printf("\n");
    fflush(stdout);
    return 0;
}
