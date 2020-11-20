#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "textmenu.h"
#include "process.h"
#include "message.h"
#include "semaphore.h"

/*
    TextMenu
    Manages the keyboard input and screen ouput functions
*/

static int currentProcessId = 0;

char inputChar() {
    char input = getchar();
    while (input == '\n') {
        input = getchar();
    }
    input = toupper(input);
    return input;
}

int inputPriorityInt() {
    int input;
    printf("Input Priority Int (0 - high, 1 - norm , 2 - low): ");
    scanf("%d", &input);

    while (input < 0 || input > 2) {
        printf("Invalid Int\n");
        scanf("%d", &input);
    }
    return input;
}

int inputPID() {
    int input;
    printf("Input Process ID: ");
    scanf("%d", &input);
    return input;
}

Message * inputMessage() {
    Message * message = malloc(sizeof(Message));
    if (message == NULL) {
        return NULL;
    }

    message->msg = malloc(sizeof(char) * 40);
    if (message->msg == NULL) {
        free(message);
        return NULL;
    }

    printf("Input Message (max 40 char): ");
    scanf("%s", message->msg);
    return message;
}

int inputSID() {
    int input = -1;
    printf("Input Semaphore ID (0-4): ");
    scanf("%d", &input);
    while (input < 0 || input > 4) {
        printf("Invalid Semaphore ID\n");
        printf("Input Semaphore ID (0-4): ");
        scanf("%d", &input);
    }
    return input;
}

int inputSValue() {
    int input = -1;
    printf("Input Semaphore Value (0<=): ");
    scanf("%d", &input);
    while (input < 0) {
        printf("Invalid Semaphore Value\n");
        printf("Input Semaphore Value (0<=): ");
        scanf("%d", &input);
    }
    return input;
}

void printIntroduction() {
    printf("** PCB's and Processing Scheduling Simulation **\n");
    printf("* Process Commands *\n");
    printf("C - Create, F - Fork, K - Kill, E - Exit, Q - Quantum\n");
    printf("* Message Commands *\n");
    printf("S - Send, R - Receive, Y - Reply\n");
    printf("* Semaphore Commands *\n");
    printf("N - New Semaphore, P - Semaphore P, V - Semaphore V\n");
    printf("* Info Commands *\n");
    printf("I - Procinfo, T - Totalinfo\n");
}

void printInvalidSetup() {
    printf("ERROR: Lists can't not be intialized");
}

void printInvalidCommand() {
    printf("Invalid Command\n");
}

void printProcessChange() {
    PCB * process = Process_getCurrentProcess();
    if (process == NULL) {
        return;
    }
    int pid = process->PID;
    if (currentProcessId != pid ) {
        printf("Process %d is running\n", pid);
        currentProcessId = pid;
    }

    if (process->isMessageReceived) {
        Message * message = Message_getMessage(process);
        printf("(%d -> %d): %s\n", message->sender, message->receiver, message->msg);
        Message_free(message);
        process->isMessageReceived = false;
    }
}

void printCreateReport(int pid) {
    if (pid == -1) {
        printf("FAILED: Process could not be created\n");
    } else {
        printf("SUCESS: Process %d created\n", pid);
    }
}

void printForkReport(int pid) {
    if (pid == -1) {
        printf("FAILED: Process could not be forked\n");
    } else {
        printf("SUCESS: Process %d forked\n", pid);
    }
}

void printKillReport(int pid) {
    switch (pid) {
        case -2:
            printf("FAILED: Process init could not be killed\n");
            break;
        case -1:
            printf("FAILED: Process could not be found\n");
            break;
        case 0:
            printf("SUCESS: Process init is killed\n");
            break;
        default:
            printf("SUCESS: Process %d is killed\n", pid);
    }
}

void printExitReport(int pid) {
    switch (pid) {
        case -2:
            printf("FAILED: Process init could not be exited\n");
            break;
        case -1:
            printf("SUCESS: Process init is exited\n");
            break;
        case 0:
            printf("SUCESS: Process init is running\n");
            break;
        default:
            printf("SUCESS: Process %d exited\n", pid);
    }
}

void printQuantumReport(int pid) {
    currentProcessId = pid;
    if (pid == 0) {
        printf("Process init is running\n");
    } else {
        printf("Process %d is running\n", pid);
    }
}

void printSendReport(int pid) {
    if (pid < 0) {
        printf("FAILED: Send failed\n");
    } else {
        printf("SUCESS: Process %d's message sent\n", pid);
    }
}

void printReceiveReport(int pid) {
    if (pid < 0) {
        printf("FAILED: Receive failed\n");
        return;
    }

    PCB * process = Process_getCurrentProcess();

    if (process->isMessageReceived) {
        Message * message = Message_getMessage(process);
        printf("SUCESS: (%d -> %d): %s\n", message->sender, message->receiver, message->msg);
        Message_free(message);
        process->isMessageReceived = false;
    } else {
        printf("SUCESS: Process %d is waiting for Message\n", pid);
    }
}

void printReplyReport(int pid) {
    if (pid < 0) {
        printf("FAILED: Reply failed\n");
        return;
    }
    printf("SUCESS: Process %d has been replied\n", pid);
}

void printSemaphoreNewReport(int sid, int svalue) {
    if (sid < 0) {
        printf("FAILED: Semaphore already created\n");
    } else {
        printf("SUCESS: Semaphore %d created with value %d\n", sid, svalue);
    }
}

void printSemaphorePReport(int sid) {
    if (sid < 0) {
        printf("FAILED: Semaphore not created yet\n");
    } else {
        printf("SUCESS: Semaphore %d P\n", sid);
    }
}

void printSemaphoreVReport(int sid) {
    if (sid < 0) {
        printf("FAILED: Semaphore not created yet\n");
    } else {
        printf("SUCESS: Semaphore %d V\n", sid);
    }
}

void printNumToPriority(int priority) {
    switch (priority) {
        case 0:
            printf("High");
            break;
        case 1:
            printf("Norm");
            break;
        case 2:
            printf("Low");
            break;
        default:
            break;
    }
}

void printNumToState(int state) {
    switch (state) {
        case PROCESS_RUNNING:
            printf("Running");
            break;
        case PROCESS_READY:
            printf("Ready");
            break;
        case PROCESS_BLOCKED:
            printf("Blocked");
            break;
        default:
            break;
    }
}

void procinfo(int pid) {
    PCB * process = Process_getProcess(pid);

    if (process == NULL) {
        printf("Proc info: Process %d not found\n", pid);
        return;
    }

    printf("Procinfo: PID - %d", pid);
    if (pid == 0) {
        printf(" (init)");
    }
    printf(", Priority - ");
    printNumToPriority(process->priority);
    printf(", State - ");
    printNumToState(process->state);
    printf(", Messages Waiting - %d", process->messages->count);
    printf("\n");
}

void printArray(int * array) {
    int count = array[0];
    if (count == 0) {
        printf("Empty \n");
    } else {
        printf("{");
        for (int i = 0; i < count-1; i++) {
            printf("%d, ", array[i+1]);
        }
        printf("%d}\n", array[count]);
    }
    free(array);
}

void totalinfo() {
    int * array;
    printf("* Totalinfo *\n");

    PCB * process = Process_getCurrentProcess();
    printf("Running: ");
    if (process->PID == 0) {
        printf("0 - init\n");
    } else {
        printf("%d\n", process->PID);
    }

    printf("High Ready Queue: ");
    array = Process_getQueueArray(PRIORITY_HIGH);
    printArray(array);

    printf("Norm Ready Queue: ");
    array = Process_getQueueArray(PRIORITY_NORM);
    printArray(array);

    printf("Low Ready Queue: ");
    array = Process_getQueueArray(PRIORITY_LOW);
    printArray(array);

    printf("Send Waiting List: ");
    array = Message_getQueueArray(QUEUE_SEND);
    printArray(array);

    printf("Receive Waiting List: ");
    array = Message_getQueueArray(QUEUE_RECEIVE);
    printArray(array);

    for (int i = 0; i < SEMAPHORE_COUNT; i++) {
        if (Semaphore_isEnabled(i)) {
            printf("Semaphore %d Queue: ", i);
            array = Semaphore_getQueueArray(i);
            printArray(array);
        }
    }
}