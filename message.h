#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "list.h"
#include "process.h"

typedef struct Message_s Message;
struct Message_s {
    char * msg;
    int sender;
    int receiver;
};

enum MESSAGEQUEUE {
    QUEUE_SEND,
    QUEUE_RECEIVE
};

int Message_setup();
void Message_free(void * message);

List * Message_getQueue(int num);
int * Message_getQueueArray(int num);
Message * Message_getMessage(PCB * process);

int Message_send(int pid, Message * message);
int Message_receive(Message * message);
int Message_reply(int pid, Message * message);

#endif