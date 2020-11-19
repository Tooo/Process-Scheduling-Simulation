#include <stdlib.h>

#include "semaphore.h"
#include "list.h"
#include "process.h"

static Semaphore semaphores[SEMAPHORE_COUNT];

int Sempahore_setup() {
    Semaphore * s;
    for (int i = 0; i < SEMAPHORE_COUNT; i++) {
        s = &semaphores[i];
        s->isEnabled = false;
        s->value = 0;
        s->waitingList = List_create();
        if (s->waitingList == NULL) {
            return -1;
        }
    }
    return 0;
}

List * Semaphore_getQueue(int sid) {
    return semaphores[sid].waitingList;
}

int * Semaphore_getQueueArray(int sid) {
    List * queue = Semaphore_getQueue(sid);
    return Process_QueueToArray(queue);
}

bool Semaphore_isEnabled(int sid) {
    return semaphores[sid].isEnabled;
}

int Semaphore_new(int sid) {
    Semaphore * s = &semaphores[sid];
    if (s->isEnabled) {
        return -1;
    }
    s->isEnabled = true;
    return sid;
}

int Semaphore_p(int sid) {
    Semaphore * s = &semaphores[sid];
    if (!s->isEnabled) {
        return -1;
    }

    s->value--;
    if (s->value < 0) {
        PCB * process = Process_getCurrentProcess();
        if (process->PID != 0) {
            process->state = PROCESS_BLOCKED;
        }
        List_prepend(s->waitingList, process);
        Process_changeRunningProcess();
    }
    return sid;
}

int Semaphore_v(int sid) {
    Semaphore * s = &semaphores[sid];
    if (!s->isEnabled) {
        return -1;
    }

    s->value++;
    if (s->value <= 0) {
        PCB * process = List_trim(s->waitingList);
        Process_prependToReadyQueue(process);
    }
    return sid;
}