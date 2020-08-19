#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    cpu_set_t set;
    int ret;
    size_t cpuset_size;
    CPU_ZERO(&set);
    ret = sched_getaffinity(0, sizeof(cpu_set_t), &set);
    if (ret == -1) {
        perror("get error");
    }
    for (int i = 0; i < CPU_SETSIZE; i++) {
        int cpu;
        cpu = CPU_ISSET(i, &set);
        printf ("cpu = %i is %s\n", i, cpu ? "set" : "unset");
    }
    CPU_CLR(0, &set);
    ret = sched_setaffinity(0, sizeof(cpu_set_t), &set);
    if (ret == -1) {
        perror ("set error");
    }
    for (int i = 0; i < CPU_SETSIZE; i++) {
        int cpu;
        cpu = CPU_ISSET(i, &set);
        printf ("cpu = %i is %s\n", i, cpu ? "set" : "unset");
    }
    int tmp = 0, pid;
    for (int i = 0; i < 32; i++) {
        int pid = fork();
        tmp = i;
        if (pid == 0) break;
    }
    if (pid == 0 && tmp == 8) {
        CPU_ZERO(&set);
        CPU_SET(0, &set);
        ret = sched_setaffinity(0, sizeof(cpu_set_t), &set);
    }
    while (1) {
        sleep(10);
    }
    return 0;
}
