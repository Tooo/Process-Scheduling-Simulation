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

PCB * removeProcess(int num, int pid) {
    List * queue = Message_getQueue(num);
    List_first(queue);
    PCB * process = List_search(queue, Process_comparePid, &num);
    if (process == NULL) {
        return NULL;
    }
    return List_remove(queue);
}

int Message_send(int pid, Message * message) {
    if (message == NULL) {
        return -1;
    }
    message->sender = Process_getCurrentProcess()->PID;
    message->receiver = pid;

    PCB * process = removeProcess(QUEUE_RECEIVE, pid);
    if (process == NULL) {
        process = Process_getProcess(pid);
        if (process == NULL) {
            return -1;
        }
        List * messages = process->messages;
        List_prepend(messages, message);
    }

    if (process->PID != 0) {
        process->state = PROCESS_BLOCKED;
    }
    List_add(sendQueue, process);
    Process_changeRunningProcess();
    return 0;
}

int Message_receieve(Message * message) {
    PCB * process = Process_getCurrentProcess();
    List * messages = process->messages;
    if (List_count(messages) == 0) {
        if (process->PID != 0) {
            process->state = PROCESS_BLOCKED;
        }
        List_prepend(receiveQueue, process);
        Process_changeRunningProcess();
    } else {
        message = List_trim(messages);
    }
    return process->PID;
}

int Message_reply(int pid, Message * message) {
    PCB * process = removeProcess(QUEUE_SEND, pid);
    if (process == NULL) {
        return -1;
    }
    message->sender = Process_getCurrentProcess()->PID;
    message->receiver = pid;
    List * messages = process->messages;
    List_prepend(messages, message);
    return pid;
}