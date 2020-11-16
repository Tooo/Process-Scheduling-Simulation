#include "textmenu.h"
#include "process.h"

int main () {
    int priority;
    int pid;

    Process_setup();
    printIntroduction();
    while (!Process_isInitExited()) {
        char input = inputChar();
        switch (input) {
            case 'C':
                priority = inputPriorityInt();
                pid = Process_create(priority);
                printCreateReport(pid);
                break;  
            case 'F':
                pid = Process_fork();
                printForkReport(pid);
                break;
            case 'K':
                return 1;
            case 'E':
                break;
            case 'Q':
                pid = Process_quantum();
                printQuantumReport(pid);
                break;
            case 'S':
                break;
            case 'R':
                break;
            case 'Y':
                break;
            case 'N':
                break;
            case 'P':
                break;
            case 'V':
                break;
            case 'I':
                pid = inputPID();
                procinfo(pid);
                break;
            case 'T':
                totalinfo();
                break;
            default:
                printInvalidCommand();
                break;
        }
    }
    
}