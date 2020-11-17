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

enum PRIORITY {
    PRIORITY_HIGH,
    PRIORITY_NORM,
    PRIORITY_LOW
};

int Process_setup();
bool Process_isInitExited();
bool Process_comparePid(void * pcb, void * pid);
PCB * Process_getProcess(int pid);
int Process_getCurrentProcessId();


int Process_create(int priority);
int Process_fork();
int Process_kill(int pid);
int Process_exit();
int Process_quantum();

#endif