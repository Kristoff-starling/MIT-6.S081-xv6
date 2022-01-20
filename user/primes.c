#include "kernel/types.h"
#include "user/user.h"

/* static void exit_error()
{
    fprintf(2, "primes: error\n");
    exit(1);
}

int main ()
{
    int a[48], tot = 34;
    for (int i = 0; i <= 33; i ++) a[i] = i + 2;

    int p[2];

    while (tot > 0)
    {
        pipe(p);

        int pid = fork();
        if (pid < 0) exit_error();
        else if (pid == 0)
        {
            close(p[1]);
            int cur, curp; tot = -1;
            while (read(p[0], &cur, sizeof(cur)) > 0)
            {
                if (tot == -1)
                {
                    printf("prime %d\n", cur);
                    tot ++; curp = cur;
                }
                else
                {
                    if (cur % curp != 0) a[tot ++] = cur;
                }
            }
            close(p[0]);
        }
        else
        {
            close(p[0]);
            for (int i = 0; i < tot; i ++)
                write(p[1], a + i, sizeof(int));
            close(p[1]);
            int status; wait(&status);
            if (status != 0) exit_error();
            exit(0);
        }
    }

    exit(0);
} */

void process_sieve(int fd)
{
    int cur, curp;
    if (read(fd, &cur, sizeof(int)) == 0)
    {
        close(fd);
        exit(0);
    }
    curp = cur;
    printf("prime %d\n", curp);

    int p[2];
    pipe(p);

    int pid = fork();
    if (pid < 0)
    {
        fprintf(2, "primes: fork error\n");
        exit(1);
    }
    else if (pid == 0)
    {
        close(fd);
        close(p[1]);
        process_sieve(p[0]);
    }
    else
    {
        close(p[0]);
        while (read(fd, &cur, sizeof(cur)) != 0)
        {
            if (cur % curp != 0)
                write(p[1], &cur, sizeof(cur));
        }
        close(p[1]);
        close(fd);
        wait(0);
    }

    exit(0);
}

int main (int argc, char *argv[])
{
    if (argc != 1)
    {
        fprintf(2, "Usage: primes\n");
        exit(1);
    }

    int p[2];
    pipe(p);

    int pid = fork();
    if (pid < 0)
    {
        fprintf(2, "primes: fork error\n");
        exit(1);
    }
    else if (pid == 0)
    {
        close(p[1]);
        process_sieve(p[0]);
    }
    else
    {
        for (int i = 2; i <= 35; i ++)
            write(p[1], &i, sizeof(int));
        close(p[0]);
        close(p[1]);
        wait(0);
    }

    exit(0);
}