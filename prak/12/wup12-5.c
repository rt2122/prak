#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <wait.h>

enum
{
    ELDER_BIT = 0x80,
    ELDER_BYTE = 0xFF000000,
    SHIFT = 3 * CHAR_BIT,
    CONT = 1,
    ENDF = 2
};

volatile int check = 0;
void 
get_check(int sig)
{
    check = 1;
}

volatile int got_sig = 0, bit;
void 
get_sig(int sig)
{
    if (sig == SIGIO) {
        got_sig = ENDF;
        return;
    }
    got_sig = CONT;
    bit = 0;
    if (sig == SIGUSR2) {
        bit = 1;
    }
}

typedef unsigned char uchar;

void 
send_byte(uchar byte, sigset_t *oldmask, int gs)
{
    uchar cur_bit = ELDER_BIT;
    while (cur_bit) {
        int sig = (cur_bit & byte) ? SIGUSR2 : SIGUSR1; 
        kill(gs, sig);
        while (!check) {
            sigsuspend(oldmask);
        }
        check = 0;
        cur_bit >>= 1;
    }
}

int io = 0;
uchar 
recv_byte(sigset_t *oldmask, int pid_check)
{
    uchar cur = 0;
    for (int cur_pos = 0; cur_pos < CHAR_BIT; ++cur_pos) {
        while (!got_sig) {
            sigsuspend(oldmask);
        }
        if (got_sig == ENDF) {
            fflush(stdout);
            _exit(0);
        }
        got_sig = 0;
        cur = (cur << 1) | bit;
        kill(pid_check, SIGALRM);
    }
    return cur;
}

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        _exit(0);
    }
    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGIO);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);
    struct sigaction ssa = { .sa_handler = get_sig, .sa_flags = SA_RESTART };
    sigaction(SIGIO, &ssa, NULL);
    sigaction(SIGUSR1, &ssa, NULL);
    sigaction(SIGUSR2, &ssa, NULL);
    int gs = fork();
    if (!gs) {   
        int ss = 0, ppid = getppid();
        for (int i = 0; i < sizeof(ss); ++i) {
            uchar byte = recv_byte(&oldmask, ppid);
            ss = (ss << CHAR_BIT) | byte;
        }
        kill(ss, SIGALRM);
        while (4) {
            uchar cur = recv_byte(&oldmask, ss);
            write(1, &cur, sizeof(cur));
        }
        _exit(0);
    }

    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);
    sigaction(SIGALRM, &(struct sigaction){ .sa_handler = get_check, 
                .sa_flags = SA_RESTART}, NULL);

    int ss = fork();
    if (!ss) {
        int fd = open(argv[1], O_RDONLY, 0);
        uchar tmp;
        
        while (!check) {
            sigsuspend(&oldmask);
        }
        check = 0;
        
        while (read(fd, &tmp, sizeof(tmp)) > 0) {
            send_byte(tmp, &oldmask, gs);
        }
        kill(gs, SIGIO);
        close(fd);
        _exit(0);
    }

    unsigned b_mask = ELDER_BYTE, pid = ss, shift = SHIFT;
    while (b_mask) {
        send_byte((pid & b_mask) >> shift, &oldmask, gs);
        shift -= CHAR_BIT;
        b_mask >>= CHAR_BIT;
    }

    while (wait(NULL) > 0) {}
    _exit(0);
}
