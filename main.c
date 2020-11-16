#include "textmenu.h"
#include "process.h"

int main () {
    Process_setup();
    printIntroduction();
    while (1) {
        char input = inputChar();
        switch (input) {
            case 'C':
                break;  
            case 'F':
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