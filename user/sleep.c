#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int
main (int argc, char* argv[]) {
    if (argc <= 1 || argc > 2) {
        fprintf(2, "usage: sleep $time\n");
        exit(1);
    }
    int n = atoi(argv[1]);
    if (n < 0)
        n = 0;
    sleep(n);
    exit(0);
}