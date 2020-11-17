#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "textmenu.h"
#include "process.h"

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
    printf("Input Priority Int (0 - high, 1 - norm , 2 - low)\n");
    scanf("%d", &input);

    while (input < 0 || input > 2) {
        printf("Invalid Int\n");
        scanf("%d", &input);
    }
    return input;
}

int inputPID() {
    int input;
    printf("Input Process ID\n");
    scanf("%d", &input);
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

void printInvalidCommand() {
    printf("Invalid Command\n");
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
    if (pid == -1) {
        printf("FAILED: Process could not be killed\n");
    } else {
        printf("SUCESS: Process %d killed\n", pid);
    }
}

void printExitReport(int pid) {
    if (pid == -1) {
        printf("FAILED: Process init could not be exited\n");
    } else if (pid == 0) {
        printf("SUCESS: Process init exited\n");
    } else {
        printf("SUCESS: Process %d is running\n", pid);
    }
}

void printQuantumReport(int pid) {
    if (pid == 0) {
        printf("Process init is running\n");
    } else {
        printf("Process %d is running\n", pid);
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
    printf("Procinfo: PID - %d", pid);
    if (pid == 0) {
        printf(" (init)");
    }
    printf(", Priority - ");
    printNumToPriority(process->priority);
    printf(", State - ");
    printNumToState(process->state);
    printf("\n");
}

void totalinfo() {
    printf("* Totalinfo *\n");
    printf("High Ready Queue: ");
    printf("Norm Ready Queue: ");
    printf("Low Ready Queue: ");

    printf("Send Waiting Queue: ");
    printf("Receieve Waiting Queue: ");
}