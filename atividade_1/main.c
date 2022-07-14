#include <stdio.h>
#include "pids.h"

int main() {
    // initiate pid map
    if (allocate_map() == -1) {
        printf("unable to create the pid map\n");
    }

    // sample pid for feature release
    int pid1, pid2;

    // allocate pids
    for (int i = 0; i < 1000; i++) {
        int pid = allocate_pid();
        if (i == 3) pid1 = pid;
        if (i == 4) pid2 = pid;
        printf("PID: %d\n", pid);
    }

    // release pids
    release_pid(pid1);
    release_pid(1000);
    release_pid(pid2);

}