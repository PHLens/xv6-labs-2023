#include "kernel/types.h"
#include "user.h"

__attribute__((noreturn))
void primes(int* left) {
    int p1[2];
    pipe(p1);
    int n;
    if (read(left[0], &n, sizeof(int))) { // only create child process when pipe has data.
        if(fork() == 0) { // child process read pipe recursively.
            close(p1[1]);
            primes(p1);
        } else { // parent process write pipe.
            int b;
            printf("prime %d\n", n);
            while(read(left[0], &b, sizeof(int))) {
                if (b % n != 0) {
                    write(p1[1], &b, sizeof(int));
                }
            }
            close(p1[1]);
        }
    }
    wait(0); // wait for child exit.(keep print order.)
    exit(0);
}

int
main (int argc, char* argv[]) {
    int p[2];
    pipe(p);
    if (fork() == 0) {
        close(p[1]);
        primes(p);
    } else {
        for (int i = 2; i <= 35; i++){
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
    }
    wait(0); // wait for child exit.(keep print order.)
    exit(0);
}
