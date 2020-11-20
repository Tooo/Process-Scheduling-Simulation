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

void Message_free(void * message) {
    Message * message_node = message;
    message_node->msg = NULL;
    message_node->sender = 0;
    message_node->receiver = 0;
    free(message);
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
            return -1;
        }
        prependMessage(receiverProcess, message);
    } else {
        receiverProcess->isMessageReceived = true;
        Process_prependToReadyQueue(receiverProcess);
    }

    if (senderProcess->PID != 0) {
        senderProcess->state = PROCESS_BLOCKED;
    }
    List_add(sendQueue, senderProcess);
    Process_changeRunningProcess();
    return senderProcess->PID;
}

int Message_receive(Message * message) {
    PCB * process = Process_getCurrentProcess();
    List * messages = process->messages;
    if (List_count(messages) == 0) {
        if (process->PID != 0) {
            process->state = PROCESS_BLOCKED;
        }
        List_prepend(receiveQueue, process);
        Process_changeRunningProcess();
        message = NULL;
    } else {
        message = Message_getMessage(process);
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
        return -1;
    }
    message->sender = Process_getCurrentProcess()->PID;
    message->receiver = pid;
    prependMessage(process, message);
    process->isMessageReceived = true;
    Process_prependToReadyQueue(process);
    return pid;
}