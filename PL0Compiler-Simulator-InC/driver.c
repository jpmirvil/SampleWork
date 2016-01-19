#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
James Mirvil & Atul Thomas
COP 3402
Project 4
PL/0 Compiler
*/

void scanner(int printScanner);
void parser(int printParser);
void vm(int printVM);

int main(int argc, char* argv[])
{
    int printScanner = 0;
    int printParser = 0;
    int printVM = 0;
    int i = 1;

    //Check to console commands -l, -a, or -v
    while(i < argc && i <= 4)
    {
        if(strcmp("-l", argv[i]) == 0)
        {
            printScanner = 1;
        }
        else if(strcmp("-a", argv[i]) == 0)
        {
            printParser = 1;
        }
        else if(strcmp("-v", argv[i]) == 0)
        {
            printVM = 1;
        }
        i++;
    }
    //Call Scanner
    scanner(printScanner);
    //Call parser
    parser(printParser);
    //call virtual machine
    vm(printVM);
    return 0;
}

