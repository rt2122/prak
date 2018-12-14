#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

enum { BUF = 100 };
const char code[] = "#!/usr/bin/python3\nx = int(input())\nprint((x * (x + 1) // 2) %% %d)\n";

int
main(int argc, char *argv[])
{
    char *out = argv[1];
    int mod;
    sscanf(argv[2], "%d", &mod);
    char text[BUF];
    int fd = open(out, O_CREAT | O_WRONLY | O_TRUNC, 0751);
    int size = snprintf(text, sizeof(text), code, mod);
    if (size >= sizeof(text)) {
        _exit(1);
    }
    write(fd, text, size); 
    close(fd);
    _exit(0);
}
