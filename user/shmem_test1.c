#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void)
{
    int ppid = getpid();
    char *shared_mem = malloc(4096);
    int pid = fork();
    if (pid < 0)
    {
        printf("fork failed\n");
        exit(1);
    }
    if (pid == 0)
    {
        sleep(2);
        printf("Child process\n");
        pid = getpid();
        uint64 addr = map_shared_pages(ppid, pid, (uint64)shared_mem, 4096);
        printf("Shared memory content: %s\n", (char *)addr);
        exit(0);
    }
    else
    {
        printf("Parent process\n");
        strcpy(shared_mem, "Hello child");
        wait(0);
    }

    exit(0);
}