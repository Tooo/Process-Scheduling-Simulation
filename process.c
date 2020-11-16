#include <stdlib.h>

#include "process.h"
#include "list.h"

static List * highQueue;
static List * normQueue;
static List * lowQueue;

static PCB init;
static PCB * runningProcess;
static int processInt = 0;

int Process_setup() {
    init.PID = processInt++;
    init.state = PROCESS_RUNNING;
    init.priority = 0;

    highQueue = List_create();
    if (highQueue == NULL) {
        return 1;
    }

    normQueue = List_create();
    if (normQueue == NULL) {
        return 2;
    }

    lowQueue = List_create();
    if (lowQueue == NULL) {
        return 3;
    }

    return 0;
}

bool Process_isInitExited() {
    if (init.state == PROCESS_BLOCKED) {
        return true;
    } else {
        return false;
    }
}

int processToReadyQueue(PCB * process) {
    process->state = PROCESS_READY;

    switch (process->priority) {
        case 0:
            return List_prepend(highQueue, process);
        case 1:
            return List_prepend(normQueue, process);
        case 2:
            return List_prepend(lowQueue, process);
        default:
            return 2;
    }

}

void fromInitRunProcess(PCB * process) {
    init.state = PROCESS_READY;
    process->state = PROCESS_RUNNING;
    runningProcess = process;
}

int Process_create(int priority) {
    int result;
    PCB * process = malloc(sizeof(PCB));

    if (process == NULL) {
        return -1;
    }

    process->PID = processInt++;
    process->priority = priority;
    
    if (init.state == PROCESS_RUNNING) {
        fromInitRunProcess(process);
    } else {
        result = processToReadyQueue(process);
    }

    if (result != 0) {
        free(process);
        processInt--;
        return 1;
    } 

    return process->PID;
}

int Process_fork() {
    int result;
    if (init.state == PROCESS_RUNNING) {
        return -1;
    }

    PCB * process = malloc(sizeof(PCB));

    if (process == NULL) {
        return -1;
    }

    process->PID = processInt++;
    process->priority = runningProcess->priority;
    result = processToReadyQueue(process);

    if (result != 0) {
        free(process);
        processInt--;
        return -1;
    }

    return process->PID;
}

void Process_kill(int pid) {

}

void Process_exit() {

}

void Process_quantum() {

}