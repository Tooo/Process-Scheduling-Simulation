#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "list.h"

typedef struct Message_s Message;
struct Message_s {
    char * message;
    int sender;
};

enum MESSAGEQUEUE {
    QUEUE_SEND,
    QUEUE_RECEIVE
};

int Message_setup();

List * Message_getQueue(int num);
int * Message_getQueueArray(int num);

int Message_send(int pid, char * msg);
int Message_receieve();
int Message_reply(int pid, char * msg);

#endif