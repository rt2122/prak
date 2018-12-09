#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

const char code[] = 
"#include <stdio.h>\n"
"enum { MOD = %d };\n"
"int"
"main(void)"
"{"
    "int x, sum = 0;"
    "scanf(\"%%d\", &x);"
    "for (int i = 1; i <= x; ++i) {"
        "sum = (sum + i) % MOD;"
    "}"
    "printf(\"%%d\n\", sum);"
    "return 0;"
"}";

int
main(int argc, char *argv[])
{
    char *out = argv[1];
    int mod;
    sscanf(argv[2], "%d", &mod);
    
    char tmp[] = "XXXXXX";
    mktemp(tmp);
    int fd = open(tmp, O_WRONLY, 0751);
    FILE *file = fdopen(fd, "w");
    fclose(file);
    
    if (!fork()) {
        execlp("gcc", "gcc", "-std=gnu11", tmp, "-o", out, NULL);
        _exit(1);
    }
    
    _exit(0);
}
