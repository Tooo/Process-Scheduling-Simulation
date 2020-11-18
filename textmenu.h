#ifndef _TEXTMENU_H_
#define _TEXTMENU_H_

char inputChar();
int inputPriorityInt();
int inputPID();

void printIntroduction();
void printInvalidSetup();
void printInvalidCommand();

void printCreateReport(int pid);
void printForkReport(int pid);
void printKillReport(int pid);
void printExitReport(int pid);
void printQuantumReport(int pid);

void printSendReport(int pid);
void printReceiveReport(int pid);
void printReplyReport(int pid);

void procinfo(int pid);
void totalinfo();

#endif