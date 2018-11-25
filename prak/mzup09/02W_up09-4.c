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
const char other_dir[] = "/tmp";

int create_name(char *buf) {
    snprintf(buf, PATH_MAX, "%d", getpid());
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
    char name_tmp[PATH_MAX], full_path[PATH_MAX], path[PATH_MAX];
    create_name(name_tmp);
    char *ch = find_env(env_dir[0]);
    if (!ch) {
        ch = find_env(env_dir[1]);
    }
    if (!ch) {
        snprintf(path, PATH_MAX, "%s", other_dir);
    } else {
        snprintf(path, PATH_MAX, "%s", ch);
    }
    int fd;

    if (snprintf(full_path, PATH_MAX, "%s/%s", path, name_tmp) > PATH_MAX) {
        return 1;
    }
    if ((fd = open(full_path, O_CREAT | O_WRONLY | O_EXCL, 0700)) < 0 ) {
        return 1;
    }
    write(fd, starting, strlen(starting));
    for (int i = 1; i < argc; i++) {
        write(fd, argv[i], strlen(argv[i]));
        if (i != argc - 1) {
            write(fd, sign, strlen(sign));
        } else {
            write(fd, rem, strlen(rem));
            write(fd, full_path, strlen(full_path));
            write(fd, ending, strlen(ending));
        }
    }
    close(fd);
    execlp("/bin/sh", "sh", "-c", full_path, NULL);
    _exit(1);
}
