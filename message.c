#include <stdlib.h>

#include "message.h"
#include "list.h"
#include "process.h"

static List * sendQueue;
static List * receiveQueue;

int Message_setup() {
    sendQueue = List_create();
    if (sendQueue == NULL) {
        return 1;
    }

    receiveQueue = List_create();
    if (receiveQueue == NULL) {
        return 2;
    }

    return 0;
}

List * Message_getQueue(int num) {
    switch (num) {
        case QUEUE_SEND:
            return sendQueue;
        case QUEUE_RECEIVE:
            return receiveQueue;
        default:
            return NULL;
    }
}

int Message_send(int pid, char * msg) {
    return 0;
}

int Message_receieve() {
    PCB * process = Process_getCurrentProcess();

    if (process->message == NULL) {
        if (process->PID != 0) {
            process->state = PROCESS_BLOCKED;
        }
        List_add(receiveQueue, process);
        Process_changeRunningProcess();
    } 
    return process->PID;
}

int Message_reply(int pid, char * msg) {
    return 0;
}