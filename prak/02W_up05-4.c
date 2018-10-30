#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

int myaccess(const struct stat *stb, const struct Task *task, int access) {
    if (task->uid == 0) {
        return 1;
    }
    if (stb->st_uid == task->uid) {
        return (((stb->st_mode) >> 6) & access) == access;
    }
    for (int i = 0; i < task->gid_count; i++) {
        if (task->gids[i] == stb->st_gid) {
            return (((stb->st_mode) >> 3) & access) == access;
        }
    }
    return ((stb->st_mode) & access) == access;
}
