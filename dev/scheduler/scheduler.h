#ifndef dql_scheduler_h
#define dql_scheduler_h

typedef void* scheduler_t;

void Execute(const char* path, char* const envp[]);

typedef struct 
{
    scheduler_t(*create)();
    void(*free)(scheduler_t);
} scheduler_api_t;

extern const scheduler_api_t Scheduler;

#endif // dql_scheduler_h
