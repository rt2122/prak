#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/limits.h>

extern char **environ;

const char *starting = "#!/usr/bin/python3 -E\nimport os\nprint(";
const char *sign = "*";
const char *rem = ")\nos.remove(\"";
const char *ending = "\")";
const char *env_dir[] = { "XDG_RUNTIME_DIR", "TMPDIR" };
const char *other_dir = "/tmp";

int create_name(char *buf) {
    static int count = 0;
    snprintf(buf, PATH_MAX, "%d%d", count, getpid());
    count++;
    return 0;
}

char *find_env(const char *val)
{
    int length = strlen(val);
    for (int i = 0; environ[i]; i++) {
        if (!strncmp(environ[i], val, length)) {
            return environ[i] + length + 1;
        }
    }
    return NULL;

}

int
main(int argc, char **argv)
{
    char *path = find_env(env_dir[0]);
    if (!path) {
        path = find_env(env_dir[1]);
        if (!path) {
            char *buf = "/tmp";
            path = buf;
        }
    }
    char name_tmp[PATH_MAX], full_path[PATH_MAX];
    int fd;
    create_name(name_tmp);
    snprintf(full_path, PATH_MAX, "%s/%s", path, name_tmp);
    while ((fd = open(full_path, O_CREAT | O_WRONLY | O_EXCL, 0700)) < 0 ) {
        create_name(name_tmp);
        snprintf(full_path, PATH_MAX, "%s/%s", path, name_tmp);
    }
    FILE *f = fdopen(fd, "w");
    //write(fd, starting, strlen(starting));
    fprintf(f, "#!/usr/bin/python3 -E\nimport os\nprint(");
    for (int i = 1; i < argc; i++) {
        //write(fd, argv[i], strlen(argv[i]));

        if (i != argc - 1) {
            fprintf(f, "%s*", argv[i]);
            //write(fd, sign, strlen(sign));
        } else {
            fprintf(f, "%s)\nos.remove(\"%s\")\n", argv[i], full_path);
            //write(fd, rem, strlen(rem));
            //write(fd, full_path, strlen(full_path));
            ///write(fd, ending, strlen(ending));
        }
    }
    fclose(f);
    close(fd);
    char *args[] = { "sh", "-c", full_path, NULL };
    execve("/bin/sh", args, NULL);
    _exit(1);
}