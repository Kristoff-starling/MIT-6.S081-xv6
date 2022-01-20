#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main (int argc, char *argv[])
{
    if (argc == 1)
    {
        fprintf(2, "Usage: xargs command...\n");
        exit(1);
    }

    char *cargv[MAXARG]; int cargc = argc - 1, Argc = argc - 1;
    char ch, buf[64]; int buf_p = 0;
    
    for (int i = 0; i < Argc; i ++) cargv[i] = argv[i + 1];

    while (read(0, &ch, 1) == 1)
    {
        switch (ch)
        {
            case ' ':
                buf[buf_p] = 0;
                cargv[cargc] = malloc(buf_p + 1); 
                strcpy(cargv[cargc ++], buf);
                buf_p = 0;

                break;
            
            case '\n':
                buf[buf_p] = 0;
                cargv[cargc] = malloc(buf_p + 1);
                strcpy(cargv[cargc ++], buf);
                buf_p = 0;

                cargv[cargc] = 0;
                int pid = fork();
                if (pid < 0)
                    fprintf(2, "xargs: fork error\n");
                else if (pid == 0)
                {
                    exec(cargv[0], cargv);
                    fprintf(2, "xargs: exec error\n");
                    exit(1);
                }
                else
                {
                    wait(0);
                    for (int i = Argc; i < cargc; i ++) free(cargv[i]);
                    cargc = Argc;
                }

                break;

            default:
                buf[buf_p ++] = ch;

                break;
        }
    }

    exit(0);
}