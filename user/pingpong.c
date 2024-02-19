#include "kernel/types.h"
#include "user.h"

int
main (int argc, char* argv[]) {
    int p[2];
    char buf[1];
    pipe(p);
    if (fork() == 0) {
        int pid = getpid();
        read(p[0], buf, 1);
        printf("%d: received ping\n", pid);
        close(p[0]);
        write(p[1], buf, 1);
        close(p[1]);
        exit(0);
    } else {
        int pid = getpid();
        write(p[1], "1", 1);
        close(p[1]);
        read(p[0], buf, 1);
        close(p[0]);
        printf("%d: received pong\n", pid);
    }
    return 0;
}