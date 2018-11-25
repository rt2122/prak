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

enum
{
    USER_SHIFT = 6,
    GROUP_SHIFT = 3
};

int
ret_acc(int mode, int shift, int acc)
{
    return ((mode >> shift) & acc) == acc;
}

int check_group(unsigned *gids, int gid_count, int id)
{
    for (int i = 0; i < gid_count; i++) {
        if (gids[i] == id) {
            return 1;
        }
    }
    return 0;
}

int
myaccess(const struct stat *stb, const struct Task *task, int access)
{
    if (task->uid == 0) {
        return 1;
    }
    if (stb->st_uid == task->uid) {
        return ret_acc(stb->st_mode, USER_SHIFT, acc);
    }
    if (check_group(task->gids, task->gid_count, stb->st_gid)) {
        return ret_acc(stb->st_mode, GROUP_SHIFT, access);
    }
    return ret_acc(stb->st_mode, 0, access);
}