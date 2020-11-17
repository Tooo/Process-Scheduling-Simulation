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

void Message_send(int pid, char * msg) {
    
}

void Message_receieve() {
    
}

void Message_reply(int pid, char * msg) {

}