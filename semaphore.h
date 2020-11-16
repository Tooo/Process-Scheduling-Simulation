#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_
#include "list.h"

typedef struct Semaphore_s Semaphore;
struct Semaphore_s {
    int sid;
    List * waitingList;
};

void Semaphore_new(int semaphore);

void Semaphore_p(int semaphore);

void Semaphore_v(int semaphore);

#endif