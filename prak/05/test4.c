#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "01R_up05-4.c"

int
main(void)
{
    const struct stat stb = { .st_uid = 0, .st_gid = 0, .st_mode = 0666};
    const struct Task t = { .uid = };
}