#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <wait.h>

int ss, gs;

enum
{
    ELDER_BIT = 0x80,
    ELDER_BYTE = 0xFF000000,
    SHIFT = 3 * CHAR_BIT,
    CONT = 1,
    ENDF = 2
};

int check = 0;
void 
get_check(int sig)
{
    check = 1;
}

int got_sig = 0, bit;
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
send_byte(uchar byte, sigset_t *oldmask)
{
    uchar cur_bit = ELDER_BIT;
    while (cur_bit) {
        int sig = (cur_bit & byte) ? SIGUSR2 : SIGUSR1; 
        kill(gs, sig);
        printf("4\n");
        while (!check) {
            sigsuspend(oldmask);
        }
        check = 0;
        cur_bit >>= 1;
    }
}

int io = 0;
uchar 
recv_byte(sigset_t *oldmask, unsigned pid_check)
{
    uchar cur = 0;
    for (int cur_pos = 0; cur_pos < CHAR_BIT; ++cur_pos) {
        printf("#\n");
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
    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGIO);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);
    sigaction(SIGIO, &(struct sigaction){ .sa_handler = get_sig, 
            .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGUSR1, &(struct sigaction){ .sa_handler = get_sig, 
            .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGUSR2, &(struct sigaction){ .sa_handler = get_sig, 
            .sa_flags = SA_RESTART }, NULL);
    gs = fork();
    if (!gs) {   
        

        unsigned sspid = 0, ppid = getppid();
        
        printf("gs:waiting for parent %d\n", SIGIO);
        while (!got_sig) {
            sigsuspend(&oldmask);
        }
        got_sig = 0;

        for (int i = 0; i < sizeof(sspid); ++i) {
            printf("gs:waiting for pid\n");
            sspid = (sspid << CHAR_BIT) | recv_byte(&oldmask, ppid);
        }
        
        while (4) {
            printf("gs:waiting for bytes\n");
            uchar cur = recv_byte(&oldmask, sspid);
            write(1, &cur, sizeof(cur));
        }
        _exit(0);
    }

    sigaction(SIGALRM, &(struct sigaction){ .sa_handler = get_check, 
                .sa_flags = SA_RESTART}, NULL);
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);

    ss = fork();
    if (!ss) {
        printf("ss:gs = %d\n", gs);
        
        int fd = open(argv[1], O_RDONLY, 0);
        uchar tmp;
        
        printf("ss:waiting for gs\n");
        while (!check) {
            sigsuspend(&oldmask);
        }
        check = 0;
        
        while (read(fd, &tmp, sizeof(tmp)) > 0) {
            printf("ss:waiting to send\n");
            send_byte(tmp, &oldmask);
        }
        kill(gs, SIGIO);
        close(fd);
        _exit(0);
    }

    unsigned b_mask = ELDER_BYTE, pid = ss, shift = SHIFT;
    
    kill(gs, SIGUSR1);
    while (b_mask) {
        printf("father:sending bytes\n");
        send_byte((pid & b_mask) >> shift, &oldmask);
        shift -= CHAR_BIT;
        b_mask >>= CHAR_BIT;
        printf("#%d\n", (pid & b_mask) >> shift);
    }

    while (wait(NULL) > 0) {}
    _exit(0);
}
