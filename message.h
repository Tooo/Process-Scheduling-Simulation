#ifndef _MESSAGE_H_
#define _MESSAGE_H_

void Message_send(int pid, char * msg);

void Message_receieve();

void Message_reply(int pid, char * msg);

#endif