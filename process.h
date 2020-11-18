#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdbool.h>
#include "list.h"

typedef struct PCB_s PCB;
struct PCB_s {
    int PID;
    int priority;
    int state;
    List * messages;
    bool isMessageReceived;
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

PCB * Process_getCurrentProcess();
bool Process_comparePid(void * pcb, void * pid);
PCB * Process_getProcess(int pid);
PCB * Process_removeProcess(int pid);
int Process_prependToReadyQueue(PCB * process);

int * Process_QueueToArray(List * queue);
int * Process_getQueueArray(int priority);

void Process_changeRunningProcess();

int Process_create(int priority);
int Process_fork();
int Process_kill(int pid);
int Process_exit();
int Process_quantum();

#endif