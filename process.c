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
    runningProcess = &init;

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

bool Process_comparePid(void * pcb, void * pid) {
    PCB * process = pcb;
    int * num = pid;
    if (process->PID == * num) {
        return true;
    } else {
        return false;
    }
}

PCB * Process_getProcess(int pid) {
    if (init.PID == pid) {
        return &init;
    }

    if (runningProcess->PID == pid) {
        return runningProcess;
    }

    List_first(highQueue);
    PCB * process = List_search(highQueue, Process_comparePid, &pid);
    if (process != NULL) {
        return process;
    }

    List_first(normQueue);
    process = List_search(normQueue, Process_comparePid, &pid);
    if (process != NULL) {
        return process;
    }

    List_first(lowQueue);
    process = List_search(lowQueue, Process_comparePid, &pid);
    if (process != NULL) {
        return process;
    }

    return NULL;
}

PCB * Process_removeProcess(int pid) {
    if (init.PID == pid) {
        return &init;
    }

    if (runningProcess->PID == pid) {
        return runningProcess;
    }

    List_first(highQueue);
    PCB * process = List_search(highQueue, Process_comparePid, &pid);
    if (process != NULL) {
        return List_remove(highQueue);
    }

    List_first(normQueue);
    process = List_search(normQueue, Process_comparePid, &pid);
    if (process != NULL) {
        return List_remove(highQueue);
    }

    List_first(lowQueue);
    process = List_search(lowQueue, Process_comparePid, &pid);
    if (process != NULL) {
        return List_remove(highQueue);
    }

    return NULL;
}

int Process_getCurrentProcessId() {
    return runningProcess->PID;
}

int processToReadyQueue(PCB * process) {
    process->state = PROCESS_READY;

    switch (process->priority) {
        case PRIORITY_HIGH:
            return List_prepend(highQueue, process);
        case PRIORITY_NORM:
            return List_prepend(normQueue, process);
        case PRIORITY_LOW:
            return List_prepend(lowQueue, process);
        default:
            return 2;
    }
}

void changeRunningProcess() {
    if (List_count(highQueue) != 0) {
        runningProcess = List_trim(highQueue);
    } else if (List_count(normQueue) != 0) {
        runningProcess = List_trim(normQueue);
    } else if (List_count(lowQueue) != 0) {
        runningProcess = List_trim(lowQueue);
    } else {
        runningProcess = &init;
    }
    runningProcess->state = PROCESS_RUNNING;
}

bool isAllListsEmpty() {
    if (List_count(highQueue) != 0) {
        return false;
    } else if (List_count(normQueue) != 0) {
        return false;
    } else if (List_count(lowQueue) != 0) {
        return false;
    } else {
        return true;
    }
}

int Process_create(int priority) {
    PCB * process = malloc(sizeof(PCB));

    if (process == NULL) {
        return -1;
    }

    process->PID = processInt++;
    process->priority = priority;
    
    if (init.state == PROCESS_RUNNING) {
        init.state = PROCESS_READY;
        process->state = PROCESS_RUNNING;
        runningProcess = process;
    } else {
        int result = processToReadyQueue(process);
        if (result != 0) {
            free(process);
            processInt--;
            changeRunningProcess();
            return -1;
        } 
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

int Process_kill(int pid) {
    if (pid == 0) {
        if (isAllListsEmpty() && init.state != PROCESS_READY) {
            init.state = PROCESS_BLOCKED;
            return pid;
        } else {
            return -2;
        }
    }

    if (pid == runningProcess->PID) {
        free(runningProcess);
        changeRunningProcess();
    } else {
        PCB * process = Process_removeProcess(pid);

        if (process == NULL) {
            return -1;
        }
    }
    
    return pid;
}

int Process_exit() {
    if (runningProcess->PID == 0) {
        if (isAllListsEmpty()) {
            init.state = PROCESS_BLOCKED;
            return -1;
        } else {
            return -2;
        }
    } 

    free(runningProcess);
    changeRunningProcess();
    return runningProcess->PID;
}

int Process_quantum() {
    processToReadyQueue(runningProcess);
    changeRunningProcess();
    return runningProcess->PID;
}