#include <stdlib.h>

#include "process.h"
#include "list.h"
#include "message.h"
#include "semaphore.h"

/*
    Process
    Manages processes for simulation
*/

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
    init.messages = List_create();
    init.isMessageReceived = false;
    if (init.messages == NULL) {
        return 1;
    }

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

void freeProcess(PCB* process) {
    process->PID = 0;
    process->priority = 0;
    process->state = 0;
    List_free(process->messages, Message_free);
    free(process);
}

bool Process_isInitExited() {
    if (init.state == PROCESS_BLOCKED) {
        return true;
    } else {
        return false;
    }
}

bool Process_isInitRunning() {
    if (init.state == PROCESS_RUNNING) {
        return true;
    } else {
        return false;
    }
}

PCB * Process_getCurrentProcess() {
    return runningProcess;
}

List * getProcessQueue(int priority) {
    switch (priority) {
        case PRIORITY_HIGH:
            return highQueue;
        case PRIORITY_NORM:
            return normQueue;
        case PRIORITY_LOW:
            return lowQueue;
        default:
            return NULL;
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

PCB * searchProcess(int pid, List ** queue) {
    if (init.PID == pid) {
        queue = NULL;
        return &init;
    }

    if (runningProcess->PID == pid) {
        queue = NULL;
        return runningProcess;
    }

    PCB * process;

    for (int i = 0; i < 3; i++) {
        * queue = getProcessQueue(i);
        List_first(*queue);
        process = List_search(*queue, Process_comparePid, &pid);
        if (process != NULL) {
            return process;
        }
    }

    for (int i = 0; i < 2; i++) {
        * queue = Message_getQueue(i);
        List_first(*queue);
        process = List_search(*queue, Process_comparePid, &pid);
        if (process != NULL) {
            return process;
        }
    }

    for (int i = 0; i < SEMAPHORE_COUNT; i++) {
        *queue = Semaphore_getQueue(i);
        List_first(*queue);
        process = List_search(*queue, Process_comparePid, &pid);
        if (process != NULL) {
            return process;
        }
    }

    queue = NULL;
    return NULL;
}

int Process_prependToReadyQueue(PCB * process) {
    if (process->PID == 0) {
        return 0;
    }

    process->state = PROCESS_READY;
    List * queue = getProcessQueue(process->priority);

    if (queue == NULL) {
        return 1;
    }
    return List_prepend(queue, process);
}

PCB * Process_getProcess(int pid) {
    List * queue = NULL;
    return searchProcess(pid, &queue);
}

PCB * Process_removeProcess(int pid) {
    List * queue = highQueue;
    PCB * process = searchProcess(pid, &queue);

    if (process == NULL) {
        return NULL;
    }

    if (queue == NULL) {
        return process;
    }

    return List_remove(queue);
}

int * Process_QueueToArray(List * queue) {
    PCB * process;
    int count = List_count(queue);
    if (queue == NULL) {
        return NULL;
    }

    int * array = malloc(sizeof(int)*(count+1));
    if (array == NULL) {
        return NULL;
    }

    array[0] = count;
    List_last(queue);

    for (int i = 0; i < count; i++) {
        process = List_curr(queue);
        array[i+1] = process->PID;
        List_prev(queue);
    }
    return array;
}

int * Process_getQueueArray(int priority) {
    List * queue = getProcessQueue(priority);
    return Process_QueueToArray(queue);
}

void Process_changeRunningProcess() {
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
    if (runningProcess->PID != 0) {
        init.state = PROCESS_READY;
    }
}

bool isAllListsEmpty() {
    List * queue;
    for (int i = 0; i < 3; i++) {
        queue = getProcessQueue(i);
        if (List_count(queue) != 0) {
            return false;
        }
    }

    for (int i = 0; i < 2; i++) {
        queue = Message_getQueue(i);
        if (List_count(queue) != 0) {
            return false;
        }
    }

    for (int i = 0; i < SEMAPHORE_COUNT; i++) {
        queue = Semaphore_getQueue(i);
        if (List_count(queue) != 0) {
            return false;
        }
    }
    return true;
}

int Process_create(int priority) {
    PCB * process = malloc(sizeof(PCB));

    if (process == NULL) {
        return -1;
    }

    process->PID = processInt++;
    process->priority = priority;
    process->messages = List_create();
    process->isMessageReceived = false;
    
    if (init.state == PROCESS_RUNNING) {
        init.state = PROCESS_READY;
        process->state = PROCESS_RUNNING;
        runningProcess = process;
    } else {
        int result = Process_prependToReadyQueue(process);
        if (result != 0) {
            freeProcess(process);
            processInt--;
            Process_changeRunningProcess();
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
    process->messages = List_create();
    process->isMessageReceived = false;

    result = Process_prependToReadyQueue(process);
    if (result != 0) {
        freeProcess(process);
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
        freeProcess(runningProcess);
        Process_changeRunningProcess();
    } else {
        PCB * process = Process_removeProcess(pid);

        if (process == NULL) {
            return -1;
        }
        freeProcess(process);
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
    int pid = runningProcess->PID;
    freeProcess(runningProcess);
    Process_changeRunningProcess();
    return pid;
}

int Process_quantum() {
    Process_prependToReadyQueue(runningProcess);
    Process_changeRunningProcess();
    return runningProcess->PID;
}