#include <stdio.h>
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