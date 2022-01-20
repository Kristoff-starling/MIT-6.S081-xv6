#include "kernel/types.h"
#include "user/user.h"

int main (int argc, char *argv[])
{
    if (argc <= 1)
    {
        fprintf(2, "Usage: sleep numbers...\n");
        exit(1);
    }

    int sum = 0;
    for (int i = 1; i < argc; i ++)
        sum += atoi(argv[i]);

    if (sleep(sum) != 0)
    {
        fprintf(2, "sleep: error\n");
        exit(1);
    }

    exit(0);
}