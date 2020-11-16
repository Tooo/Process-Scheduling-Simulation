#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

char inputChar() {
    char input = getchar();
    while (input == '\n') {
        input = getchar();
    }
    input = toupper(input);
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