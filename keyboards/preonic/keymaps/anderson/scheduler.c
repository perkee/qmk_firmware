#include "scheduler.h"

SCHEDULED scheduled[SCHED_SIZE] = {0};
int sched_i = 0;

int run_after(int ms, SCHEDULED_CALLBACK cb, int data) {
    int index = sched_i;
    SCHEDULED sch = {.timer = timer_read(), .ms = ms, .data = data, .cb = cb};
    scheduled[sched_i] = sch;
    sched_i = (sched_i + 1) % SCHED_SIZE;
    return index;
}

void cancel(int id) {
    scheduled[id].cb = 0;
}

bool cancelled(int id) {
    return scheduled[id].cb == 0;
}

void scheduler_process(void) {
    for (int i = 0; i < SCHED_SIZE; i++) {
        if (scheduled[i].cb && timer_elapsed(scheduled[i].timer) > scheduled[i].ms) {
            SCHEDULED_CALLBACK cb = scheduled[i].cb;
            scheduled[i].cb = 0;
            cb(scheduled[i].data);
        }
    }
}

