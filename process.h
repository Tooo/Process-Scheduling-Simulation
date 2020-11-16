#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdbool.h>

typedef struct PCB_s PCB;
struct PCB_s {
    int PID;
    int priority;
    int state;
    char * message;
};

enum STATE {
    PROCESS_RUNNING,
    PROCESS_READY,
    PROCESS_BLOCKED
};

int Process_setup();

bool Process_isInitExited();

void Process_create(int priority);

void Process_fork();

void Process_kill(int pid);

void Process_exit();

void Process_quantum();

void Process_procinfo();

void Process_totalinfo();

#endif