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

enum { N_OPS = 1 };

int
dif_seq(int *seq1, int *seq2, int n1, int n2)
{
    int i1 = 0, i2 = 0;
    int *ans = seq1;
    int cur = 0;
    while (i1 < n1 && i2 < n2) {
        int b = seq1[i1] == seq2[i2] ? 0 : (seq1[i1] > seq2[i2] ? 1 : -1);
        if (seq1[i1] == seq2[i2]) {
            ans[cur++] = seq1[i1]; 
            ++i1;
            ++i2;
        } else {
            if (seq1[i1] > seq2[i2]) {
                ++i1;
            } else {
                if (seq1[i1] < seq2[i2]) {
                    ++i2;
                }
            }
        }
    }
    return cur;
}

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
    int x[10] = { 1, 1, 2, 3, 4, 5, 5, 6, 6, 7 };
    int y[10] = { 1, 1, 2, 3, 4, 5, 5, 6, 7, 7 };
    /*
    int semid = semget(key, nproc, 0660 | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        printf("error: %s\n", strerror(errno));
        exit(1);
    }
    int *val = mmap(NULL, sizeof(maxval), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    semctl(semid, 0, IPC_RMID);
    munmap(val, sizeof(maxval));
    */
    int k = dif_seq(x, y, 10, 10);
    for (int i = 0; i < k; ++i) {
        printf("%d ", x[i]);
    }
    return 0;
}
