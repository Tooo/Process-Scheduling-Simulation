#include "semaphore.h"
#include "list.h"
#include "process.h"

static Semaphore semaphores[5];

int Semaphore_new(int sid) {
    Semaphore s = semaphores[sid];
    if (s.isEnabled) {
        return -1;
    }

    s.isEnabled = true;
    s.value = 0;
    s.waitingList = List_create();
    return sid;
}

int Semaphore_p(int sid) {
    Semaphore s = semaphores[sid];
    if (!s.isEnabled) {
        return -1;
    }

    s.value--;
    if (s.value < 0) {
        PCB * process = Process_getCurrentProcess();
        process->state = PROCESS_BLOCKED;
        List_prepend(s.waitingList, process);
        Process_changeRunningProcess();
    }
    return sid;
}

int Semaphore_v(int sid) {
    Semaphore s = semaphores[sid];
    if (!s.isEnabled) {
        return -1;
    }

    s.value++;
    if (s.value <= 0) {
        PCB * process = List_trim(s.waitingList);
        Process_prependToReadyQueue(process);
    }
    return sid;
}