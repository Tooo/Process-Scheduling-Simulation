#include "textmenu.h"
#include "process.h"
#include "message.h"
#include "semaphore.h"

int main () {
    int priority;
    int pid;
    Message * message;
    int sid;
    int svalue;

    if (Process_setup() != 0) {
        printInvalidSetup();
        return 1;
    }

    if (Message_setup() != 0) {
        printInvalidSetup();
        return 1;
    }

    if (Sempahore_setup() != 0) {
        printInvalidSetup();
        return 1;
    }

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
                pid = inputPID();
                pid = Process_kill(pid);
                printKillReport(pid);
                break;
            case 'E':
                pid = Process_exit();
                printExitReport(pid);
                break;
            case 'Q':
                pid = Process_quantum();
                printQuantumReport(pid);
                break;
            case 'S':
                pid = inputPID();
                message = inputMessage();
                pid = Message_send(pid, message);
                printSendReport(pid, message);
                break;
            case 'R':
                pid = Message_receive(message);
                printReceiveReport(pid, message);
                break;
            case 'Y':
                pid = inputPID();
                message = inputMessage();
                pid = Message_reply(pid, message);
                printReplyReport(pid);
                break;
            case 'N':
                sid = inputSID();
                svalue = inputSValue();
                sid = Semaphore_new(sid, svalue);
                printSemaphoreNewReport(sid, svalue);
                break;
            case 'P':
                sid = inputSID();
                sid = Semaphore_p(sid);
                printSemaphorePReport(sid);
                break;
            case 'V':
                sid = inputSID();
                sid = Semaphore_v(sid);
                printSemaphoreVReport(sid);
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
        printProcessChange();
    }
    
}