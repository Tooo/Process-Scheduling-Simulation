#include "textmenu.h"
#include "process.h"

int main () {
    Process_setup();
    printIntroduction();
    while (!Process_isInitExited()) {
        char input = inputChar();
        switch (input) {
            case 'C':
                Process_create(inputPriorityInt());
                break;  
            case 'F':
                Process_fork();
                break;
            case 'K':
                return 1;
            case 'E':
                break;
            case 'Q':
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
                break;
            case 'T':
                break;
            default:
                printInvalidCommand();
                break;
        }
    }
    
}