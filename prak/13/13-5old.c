#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>

void
switch_seq(int **s1, int *l1, int *c1, int **s2, int *l2, int *c2)
{
    int *tmp = *s1;
    *s1 = *s2;
    *s2 = tmp;
    int l = *l1;
    *l1 = *l2;
    *l2 = l;
    l = *c1;
    *c1 = *c2;
    *c2 = l;

}

int
seq_interseq(int *seq1, int len1, int *seq2, int len2)
{
    int cur1 = 0, cur2 = 0, cur = 0;
    int *res = seq1;
    while (cur1 < len1 && cur2 < len2) {
        if (seq1[cur1] == seq2[cur2]) {
            res[cur] = seq1[cur1];
            cur++;
            cur1++;
            cur2++;
        } else {
            if (seq1[cur1] < seq2[cur2]) {
                switch_seq(&seq1, &len1, &cur1, &seq2, &len2, &cur2);    
            }
            while (seq1[cur1] > seq2[cur2] && cur2 < len2) {
                ++cur2;
            }
        }
    }
    return cur;
}

void
print_seq(int *seq, int len)
{
    for (int i = 0; i < len; ++i) {
        printf("%d ", seq[i]);
    }
    printf("\n");
}

enum { BUF = 4444 };

int
*make_map(const char *name, int *size)
{
    FILE *f = fopen(name, "r");
    int max = BUF, i = 0;
    int *buf = calloc(BUF, sizeof(int));
    while (fscanf(f, "%d", &buf[i]) == 1) {
        i++;
        if (i == max) {
            max += BUF;
            buf = realloc(buf, max * sizeof(int));
        }
    }
    int *res = mmap(NULL, i * sizeof(int), PROT_READ | PROT_WRITE, 
            MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    for (int j = 0; j < i; j++) {
        res[j] = buf[j];
    }
    free(buf);
    *size = i;
    fclose(f);
    return res;
}

enum { KEY = 4444 };

int 
get_info_proc(int i, int n, int *nseq1, int *nseq2, int *lvl, int flag)
{
    *lvl = 1;
    int mul = 1, cur = 0;
    *nseq1 = 0;
    while (cur < i) {
        *nseq1 += 2 * mul;
        if (*nseq1 + mul >= n) {
            ++*lvl;
            mul *= 2;
            *nseq1 = 0;
        }
        cur++;
    }
    *nseq2 = *nseq1 + mul;
    if (!flag) {
        return 0;
    }
    if (2 * mul > n) {
        return -1;
    }
    int ns1n = 0, ns2n = 2, lvln, next = i + 1;
    get_info_proc(next, n, &ns1n, &ns2n, &lvln, 0);
    while (ns1n != *nseq1 && ns2n != *nseq1 && next < n) {
        ++next;
        get_info_proc(next, n, &ns1n, &ns2n, &lvln, 0);
    }
    return next;
}

void
set_sem(int semid, int n, int val)
{
    for (int i = 0; i < n; ++i) {
        semctl(semid, i, SETVAL, val);
    }
}

int
main(int argc, char **argv)
{
    
    
    int n_files = argc - 1;
    struct sseq 
    {
        int *seq;
        int len;
    };
    struct sproc
    {
        int seq1, seq2, op, next;
    };
    struct sproc *proc = mmap(NULL, (n_files - 1) * sizeof(struct sproc), PROT_READ | PROT_WRITE,
            MAP_ANONYMOUS | MAP_SHARED, 0, 0); 
    for (int i = 0; i < n_files - 1; ++i) {
        int lvl;
        proc[i].next = get_info_proc(i, n_files, &(proc[i].seq1), &(proc[i].seq2), &lvl, 1);
        proc[proc[i].next].op++;
        if (i < n_files / 2) {
            proc[i].op = 1;
        }
    }
    struct sseq *seq = mmap(NULL, (n_files - 1) * sizeof(struct sseq), PROT_READ | PROT_WRITE, 
            MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    for (int i = 1; i < argc; ++i) {
        seq[i - 1].seq = make_map(argv[i], &(seq[i - 1].len));
    }
    int semid = semget(KEY, n_files - 1, 0666 | IPC_CREAT | IPC_EXCL);
    set_sem(semid, n_files - 1, 0);
    set_sem(semid, n_files / 2, 1);
    for (int i = 0; i < n_files - 1; ++i) {
        if (!fork()) {
            int nseq1 = proc[i].seq1, nseq2 = proc[i].seq2, next = proc[i].next;
            semop(semid, (struct sembuf[]){{ .sem_num = i, .sem_flg = 0, 
                    .sem_op = -proc[i].op }}, 1);
            seq[nseq1].len = seq_interseq(seq[nseq1].seq, seq[nseq1].len, 
                    seq[nseq2].seq, seq[nseq2].len);
            if (next < 0) {
                _exit(0);
            }
            semop(semid, (struct sembuf[]){{ .sem_num = next, .sem_flg = 0, 
                    .sem_op = 1}}, 1);
            _exit(0);
        }
    }
    while(wait(NULL) > 0) {}
    print_seq(seq[0].seq, seq[0].len);
    semctl(semid, 0, IPC_RMID);
    return 0;
}
