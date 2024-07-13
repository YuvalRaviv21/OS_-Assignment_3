#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PGSIZE 4096

void print_size(const char *msg)
{
    printf("%s = %d\n", msg, sbrk(0));
}

int main(void)
{
    char *shmem;
    int pid, ppid = getpid();

    print_size("Parent: initial size");
    if ((shmem = malloc(PGSIZE)) == 0)
    {
        printf("malloc failed\n");
        exit(1);
    }
    print_size("Parent: size after allocating shared memory");
    pid = fork();
    if (pid < 0)
    {
        printf("fork failed\n");
        exit(1);
    }
    if (pid == 0) // Child process
    {
        uint64 child_va;

        print_size("Child: initial size");
        ;
        if ((child_va = map_shared_pages(ppid, getpid(), (uint64)shmem, PGSIZE)) < 0)
        {
            printf("map_shared_pages failed\n");
            exit(1);
        }
        print_size("Child: size after mapping shared memory");
        ;
        strcpy((char *)child_va, "Hello daddy");
        ;
        if (unmap_shared_pages(getpid(), child_va, PGSIZE) < 0)
        {
            printf("unmap_shared_pages failed\n");
            exit(1);
        }
        print_size("Child: size after unmapping shared memory");
        ;
        if (malloc(PGSIZE) == 0)
        {
            printf("malloc failed\n");
            exit(1);
        }
        print_size("Child: size after malloc");

        exit(0);
    }
    wait(0);
    printf("Parent reads: %s\n", shmem);
    free(shmem);
    exit(0);
}