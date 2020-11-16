#include <stdlib.h>

#include "process.h"
#include "list.h"

static List * highQueue;
static List * normQueue;
static List * lowQueue;

static PCB init;
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

void Process_create(int priority) {

}

void Process_fork() {

}

void Process_kill(int pid) {

}

void Process_exit() {

}

void Process_quantum() {

}

void Process_procinfo() {

}

void Process_totalinfo() {

}