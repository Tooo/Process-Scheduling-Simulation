#include <stdlib.h>

#include "message.h"
#include "list.h"
#include "process.h"

/*
    Message
    Manages messages system for simulation
*/

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

void Message_free(void * message) {
    Message * message_node = message;
    free(message_node->msg);
    message_node->sender = 0;
    message_node->receiver = 0;
    free(message_node);
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

int * Message_getQueueArray(int num) {
    List * queue = Message_getQueue(num);
    return Process_QueueToArray(queue);
}

int prependMessage(PCB * process, Message * message) {
    List * messages = process->messages;
    return List_prepend(messages, message);
}

Message * Message_getMessage(PCB * process) {
    List * messages = process->messages;
    return List_trim(messages);
}

PCB * removeProcess(int num, int pid) {
    List * queue = Message_getQueue(num);
    List_first(queue);
    PCB * process = List_search(queue, Process_comparePid, &pid);
    if (process == NULL) {
        return NULL;
    }
    return List_remove(queue);
}

int Message_send(int pid, Message * message) {
    if (message == NULL) {
        return -1;
    }

    PCB * senderProcess = Process_getCurrentProcess();
    message->sender = senderProcess->PID;
    message->receiver = pid;

    PCB * receiverProcess = removeProcess(QUEUE_RECEIVE, pid);
    if (receiverProcess == NULL) {
        receiverProcess = Process_getProcess(pid);
        if (receiverProcess == NULL) {
            Message_free(message);
            return -1;
        }
        prependMessage(receiverProcess, message);
    } else {
        receiverProcess->isMessageReceived = true;
        prependMessage(receiverProcess, message);
        Process_prependToReadyQueue(receiverProcess);
    }

    if (Process_isInitRunning()) {
        return 0;
    }

    senderProcess->state = PROCESS_BLOCKED;
    List_add(sendQueue, senderProcess);
    Process_changeRunningProcess();
    return senderProcess->PID;
}

int Message_receive() {
    PCB * process = Process_getCurrentProcess();
    List * messages = process->messages;

    if (List_count(messages) == 0) {
        if (Process_isInitRunning()) {
            return -1;
        }

        process->state = PROCESS_BLOCKED;
        List_prepend(receiveQueue, process);
        Process_changeRunningProcess();
    } else {
        process->isMessageReceived = true;
    }
    return process->PID;
}

int Message_reply(int pid, Message * message) {
    if (message == NULL) {
        return -1;
    }

    PCB * process = removeProcess(QUEUE_SEND, pid);
    if (process == NULL) {
        Message_free(message);
        return -1;
    }
    message->sender = Process_getCurrentProcess()->PID;
    message->receiver = pid;
    prependMessage(process, message);
    process->isMessageReceived = true;
    Process_prependToReadyQueue(process);
    return pid;
}