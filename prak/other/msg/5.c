#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int
sys_check(int code, int *pids, int n, int msgid)
{
    if (code == -1) {
        printf("@");
        for (int i = 0; i < n; ++i) {
            kill(pids[i], SIGKILL);
        }
        msgctl(msgid, IPC_RMID, NULL);
        _exit(0);
    }
    return code;
}

typedef struct Buf
{
    long mtype;
    long long val1;
    long long val2;
} Message;

enum 
{ 
    MES_SIZE = sizeof(Message) - sizeof(long),
    MAX_PROC = 444
};

int
main(int argc, char *argv[])
{
    int key, n;
    long long val1, val2, maxval;
    sscanf(argv[1], "%d", &key);
    sscanf(argv[2], "%d", &n);
    sscanf(argv[3], "%lld", &val1);
    sscanf(argv[4], "%lld", &val2);
    sscanf(argv[5], "%lld", &maxval);
    
    int msgid = msgget(key, 0660 | IPC_CREAT | IPC_EXCL);
    if (msgid < 0) {
        printf("msgget: %s\n", strerror(errno));
        exit(0);
    }

    int pids[MAX_PROC];
    printf("%d\n", MES_SIZE);
    for (int i = 0; i < n; ++i) {
        pids[i] = sys_check(fork(), pids, i, msgid);
        if (!pids[i]) {
            Message mes;
            while (4) {
                msgrcv(msgid, &mes, MES_SIZE, i + 1, 0);
                long long val3 = mes.val1 + mes.val2;
                printf("%d %lld\n", i, val3);
                fflush(stdout);
                if (val3 > maxval) {
                    printf("#");
                    _exit(0);
                }
                mes.val1 = mes.val2;
                mes.val2 = val3;
                mes.mtype = val3 % n + 1;
                int er = msgsnd(msgid, &mes, MES_SIZE, 0);
                if (er < 0) {
                    printf("error: %s\n", strerror(errno));
                    printf("%ld %lld %lld\n", mes.mtype, mes.val1, mes.val2);
                    exit(0);
                }
            }
            _exit(0);
        }
    }
    Message mes;
    mes.mtype = 1;
    mes.val1 = val1;
    mes.val2 = val2;
    int er = msgsnd(msgid, &mes, MES_SIZE, 0);
    if (er < 0) {
        printf("error: %s\n", strerror(errno));
        sys_check(-1, pids, n, msgid);
    }
    int pid = wait(NULL);
    for (int i = 0; i < n; ++i) {
        if (pids[i] != pid) {
            kill(pids[i], SIGKILL);
        }
    }
    while (wait(NULL) > 0) {}
    msgctl(msgid, IPC_RMID, NULL);
    _exit(0);
}
