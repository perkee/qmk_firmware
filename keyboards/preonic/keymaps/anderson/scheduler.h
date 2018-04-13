#include "preonic.h"

// Scheduler implementation
// Usage:
//  run_after(150, cb, 0)

#define SCHED_SIZE 8

typedef void (*SCHEDULED_CALLBACK)(int);
typedef struct scheduled_t {
    int timer;
    int ms;
    int data;
    SCHEDULED_CALLBACK cb;
} SCHEDULED;

int run_after(int ms, SCHEDULED_CALLBACK cb, int data);
void cancel(int id);
bool cancelled(int id);
void scheduler_process(void);

