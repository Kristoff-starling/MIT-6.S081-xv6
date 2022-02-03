#ifdef LAB_UTIL
#include "kernel/types.h"
#include "user/user.h"

static void exit_error()
{
    fprintf(2, "pingpong: error\n");
    exit(1);
}

int main ()
{
    int p1[2], p2[2];
    pipe(p1); pipe(p2);

    int pid = fork();
    if (pid < 0) exit_error();

    if (pid == 0)
    {
        char buf[64];
        int n = read(p1[0], buf, sizeof(buf));
        if (!(n == 1 && buf[0] == 'a')) exit_error();
        printf("%d: received ping\n", getpid());
        write(p2[1], buf, 1);
        exit(0);
    }
    else
    {
        char buf[64];
        write(p1[1], "a", 1);
        int n = read(p2[0], buf, sizeof(buf));
        if (!(n == 1 && buf[0] == 'a')) exit_error();
        printf("%d: received pong\n", getpid());
    }
    
    exit(0);
}
#endif