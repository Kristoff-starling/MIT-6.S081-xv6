#ifdef LAB_UTIL
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

static char* fmtname(char *path)
{
    char *p;
    for (p = path + strlen(path); p > path && *p != '/'; p --);
    p ++;
    return p;
}

static void find(char *path, char *pattern)
{
    int fd;
    struct stat st;
    struct dirent de;
    char buf[60], *p;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        return;
    }

    switch(st.type)
    {
        case T_FILE:
            if (strcmp(fmtname(path), pattern) == 0)
                printf("%s\n", path);
            break;
        case T_DIR:
            if (strcmp(fmtname(path), pattern) == 0)
                printf("%s\n", path);

            strcpy(buf, path);
            p = buf + strlen(path);
            *p++ = '/';

            while (read(fd, &de, sizeof(de)) == sizeof(de))
            {
                if (de.inum == 0) continue;
                if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;

                strcpy(p, de.name);
                find(buf, pattern);
            }

            break;
    }

    close(fd);
}

int main (int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(2, "Usage: find directory name\n");
        exit(1);
    }

    find(argv[1], argv[2]);

    exit(0);
}
#endif