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

int 
nearest_2(int num)
{
    unsigned x = num, i = 0;
    while (x != 1) {
        x >>= 1;
        i++;
    }
    return 1U<<i;
}
struct sseq 
    {
        int *seq;
        int len;
    };
void
do_inter(struct sseq *seq, int nseq1, int nseq2)
{
    seq[nseq1].len = seq_interseq(seq[nseq1].seq, seq[nseq1].len, 
            seq[nseq2].seq, seq[nseq2].len);
}

void
work(struct sseq *seq, int st, int ed)
{
    int def = ed - st, near = ed;
    if (def > 1) {
        near = nearest_2(ed);
        if (!fork()) {
            work(seq, st, near - 1);
            _exit(0);
        }
        if (near != ed) {
            if (!fork()) {
                work(seq, near, ed);
                _exit(0);
            }
        }
        
    }
    while (wait(NULL) > 0) {}
    do_inter(seq, st, near);
}

int
main(int argc, char **argv)
{
    int n_files = argc - 1;
    if (n_files == 0) {
        return 0;
    }
    if (n_files == 1) {
        int size;
        int *seq = make_map(argv[1], &size);
        print_seq(seq, size);
        return 0;
    }
    struct sseq *seq = mmap(NULL, (n_files - 1) * sizeof(struct sseq), PROT_READ | PROT_WRITE, 
            MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    for (int i = 1; i < argc; ++i) {
        seq[i - 1].seq = make_map(argv[i], &(seq[i - 1].len));
    }
    
    work(seq, 0, n_files - 1);
    print_seq(seq[0].seq, seq[0].len);
    return 0;
}
