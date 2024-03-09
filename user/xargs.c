#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user.h"

int
main (int argc, char* argv[]) {
    char buf[512], *p, *q;
    sleep(10);  // wait 1s for data from other process ready. Warning: maybe github codespaces hardware issue.
    int total_size = read(0, buf, sizeof(buf));
    //printf("read size: %d\n", total_size);
    char* args[MAXARG];
    p = q = buf;
    while(p < buf + total_size) {
        if (*p == '\n') {
            *p = 0;
            if (argc < 2) {
                printf("%s\n", q);
                exit(0);
            }
            //printf("arg: %s\n", q);
            char** ptr = &argv[1];
            memmove(args, ptr, (argc - 1) * sizeof(char*));
            args[argc - 1] = q;
            if (fork() == 0) {
                exec(args[0], args);
                exit(0);
            }
            wait(0);
            q = p + 1;
        }
        p++;
    }
    exit(0);
}