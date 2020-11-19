#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include "list.h"
#include "stdbool.h"

#define SEMAPHORE_COUNT 5

#include "list.h"

typedef struct Semaphore_s Semaphore;
struct Semaphore_s {
    bool isEnabled;
    int value;
    List * waitingList;
};

int Sempahore_setup();

List * Semaphore_getQueue(int sid);
int * Semaphore_getQueueArray(int sid);
bool Semaphore_isEnabled(int sid);

int Semaphore_new(int sid);
int Semaphore_p(int sid);
int Semaphore_v(int sid);

#endif