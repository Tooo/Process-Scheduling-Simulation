#ifndef _MESSAGE_H_
#define _MESSAGE_H_

enum MESSAGEQUEUE {
    QUEUE_SEND,
    QUEUE_RECEIVE
};

int Message_setup();

int Message_send(int pid, char * msg);
int Message_receieve();
int Message_reply(int pid, char * msg);

#endif