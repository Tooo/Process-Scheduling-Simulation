#ifndef _TEXTMENU_H_
#define _TEXTMENU_H_

#include "message.h"

char inputChar();
int inputPriorityInt();
int inputPID();
Message * inputMessage();
int inputSID();
int inputSValue();

void printIntroduction();
void printInvalidSetup();
void printInvalidCommand();
void printProcessChange();

void printCreateReport(int pid);
void printForkReport(int pid);
void printKillReport(int pid);
void printExitReport(int pid);
void printQuantumReport(int pid);

void printSendReport(int pid, Message * message);
void printReceiveReport(int pid, Message * message);
void printReplyReport(int pid);

void printSemaphoreNewReport(int sid, int svalue);
void printSemaphorePReport(int sid);
void printSemaphoreVReport(int sid);

void procinfo(int pid);
void totalinfo();

#endif