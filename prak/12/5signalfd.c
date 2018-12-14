#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/signalfd.h>

enum
{
    LASTBIT = 1,
    MASK = 1 << 8 - 1
};

void send(char mes, pid_t addr, int sigfd)
{
    for (int i = 0; i < CHAR_BIT; ++i) {
        int sig;
        if (mes & LASTBIT == 0) {
            sig = SIGUSR1;
        } else {
            sig = SIGUSR2;
        }
        kill(addr, sig);
        struct signalfd_siginfo info;
        read(sigfd, &info, sizeof(info));
        mes >>= 1;
    }
}

char recv(pid_t addr, int sigfd)
{
    char ans = 0, shift = LASTBIT;
    for (int i = 0; i < CHAR_BIT; ++i) {
        struct signalfd_siginfo info;
        read(sigfd, &info, sizeof(info));
        if (info.ssi_signo == SIGUSR2) {
            ans |= shift;
        }
        if (info.ssi_signo == SIGIO) {
            return 0;
        }
        shift <<= 1;
        kill(addr, SIGALRM);
    }
    printf("%c", ans);
    return 1;
}

int main(int argc, char *argv[])
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);
    sigaddset(&set, SIGIO);
    sigaddset(&set, SIGALRM);
    int sigfd = signalfd(-1, &set, 0);
    int fd = open(argv[1], O_RDONLY, 0);

    pid_t pid_rcv, pid_snd;
    pid_rcv = fork();
    if (!pid_rcv) {
        pid_t addr = 0;
        int shift = 0;
        for (int i = 0; i < sizeof(addr); ++i) {
            addr |= recv(getppid(), sigfd) << shift;
            shift += CHAR_BIT;
        }
        while ((recv(addr, sigfd)));
        _exit(0);
    }

    if (!pid_snd) {
        char byte;
        while (read(fd, &byte, sizeof(byte))) {
            send(byte, pid_rcv, sigfd);
        }
        kill(pid_rcv, SIGIO);
        _exit(0);
    }
    
    for (int i = 0; i < sizeof(pid_snd); ++i) {
        send(pid_snd & MASK, pid_rcv, sigfd);
        pid_snd >>= CHAR_BIT;
    }

    _exit(0);
}
