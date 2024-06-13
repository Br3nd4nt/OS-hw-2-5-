#include "unistd.h"
#include "stdio.h"
#include <stdint.h>
#include <stdlib.h>

uint64_t fib(unsigned long n) {
    unsigned long a = 0, b = 1, c;
    if (n == 0) {
        return a;
    }
    for (unsigned long i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

uint64_t fact(unsigned long n) {
    uint64_t res = 1;
    uint64_t test = 1;
    for (unsigned long i = 1; i < n; i++) {
        test = res * i;
        if (res > test) {
            printf("OVERFLOW with n: %lu\n", i);
        }
        res = test;
    }
    return res;
}


int main(int argc, char *argv[]) {
    pid_t pid, ppid, chpid;
    unsigned long n;
    uint64_t parent_res, child_res;
    int status;
    if (argc != 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }
    n = atoi(argv[1]);
    chpid = fork();
    pid = getpid();
    ppid = getppid();
    if (chpid == -1) {
        perror("fork");
        return 1;
    } else if (chpid == 0) {
        // i am a child
        child_res = fact(n);
        printf("Child proccess\n");
        printf("Factorial(%lu): %llu\n", n, child_res);
        printf("pid: %d, ppid: %d, chpid: %d\n", pid, ppid, chpid);
        exit(EXIT_SUCCESS);
    }else {
        // i am a parent
        parent_res = fib(n);
        printf("Parent proccess\n");
        printf("Fibonacci(%lu): %llu\n", n, parent_res);
        printf("pid: %d, ppid: %d, chpid: %d\n", pid, ppid, chpid);
        // exit(EXIT_SUCCESS);
    }

    waitpid(ppid, &status, 0);
    waitpid(chpid, &status, 0);
    chpid = fork();
    pid = getpid();
    ppid = getppid();
    if (chpid == -1) {
        perror("fork");
        return 1;
    } else if (chpid == 0) {
        printf("Catalog contents from child proccess\n");
        execlp("ls", "ls", "-l", NULL);
    }
}
