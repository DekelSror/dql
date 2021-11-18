#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

#include "scheduler.h"

#define scheduler_thisify _scheduler_t* this = _this;

typedef struct
{

} _scheduler_t;

static scheduler_t Create() {}
static void Free(scheduler_t _this) {}


static char* const config[] = {"smallak", "wallak"};


void Execute(const char* path, char* const envp[]) 
{
    int current_pid = fork();
    pid_t child_pid = 0;

    if (current_pid == 0) // new process
    {
        child_pid = getpid();
        printf("child execv %d\n", child_pid);
        execve(path, config, envp);
    }
    else 
    {
        printf("parent wait %d\n", child_pid);
        pid_t wat = waitpid(-1, NULL, 0);
        printf("wat %d\n", wat);
    }
}

const scheduler_api_t Scheduler = {Create, Free}; 
