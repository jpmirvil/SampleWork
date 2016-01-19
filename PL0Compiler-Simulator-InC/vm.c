#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 10
/*
James Mirvil & Atul Thomas
COP 3402
Project 4
PL/0 Compiler
*/

typedef struct instruction
{
    int op; //opcode
    int l; // L
    int m; // M
} vmInstruction;

int stack[MAX_STACK_HEIGHT] = {0};
vmInstruction *code[MAX_CODE_LENGTH];
int activationLevel[MAX_LEXI_LEVELS] = {0};

//registers
int sp = 0;
int bp = 1;
int pc = 0;
vmInstruction *ir = 0;

int activationIndex = 0;

char* getOperationName(int op);
char* getCurrentCode(int i);
int base(int level, int b);
int storeCodeArray();
int execute(int halt);
int validateInstruction(int max);
int printSeperator(int current,int index);
void read();
void fetch();
void print();
void outputFile();
void memoryNullCheck(vmInstruction *current);
void fileNullCheck(FILE* inputFile, char* message);
void outputCodeToFile(FILE* outputFile, int numberOfinstructions);
void outputCurrentState(FILE* outputFile);

//entry point for Virtual Machine
void vm(int printVM)
{
    char fileChar;
    int numberOfinstructions, halt = 0;
    FILE* outputFile = fopen("stacktrace.txt", "w"), *inFile;

    //Make sure that we can write to file before proceeding
    fileNullCheck(outputFile, "writing stacktrace.txt");
    numberOfinstructions = storeCodeArray();

    //ensure that the file is not empty before executing program
    if(numberOfinstructions >= 0)
    {
        outputCodeToFile(outputFile, numberOfinstructions);
        //Header for section 2 of output file
        fprintf(outputFile, "\n\t\t\t\tpc\tbp\tsp\tstack\n");
        fprintf(outputFile, "Initial values\t\t\t%d\t%d\t%d\n", pc, bp, sp);
        //looping until halt conditions are met
        while(!halt)
        {
            //save the current code for this instruction
            fprintf(outputFile, "%s\t", getCurrentCode(pc));

            //begin instruction
            fetch();
            halt = validateInstruction(numberOfinstructions);
            halt = execute(halt);
            //save current state of stack
            outputCurrentState(outputFile);
            halt = validateInstruction(numberOfinstructions);
        }
    }
    else
    {
        printf("Input file does not contain code to execute program.\n");
    }
    //Only close file when program has halted
    fclose(outputFile);

    //Print of VM based on driver input
    if(printVM)
    {
        inFile = fopen("stacktrace.txt", "r");
        fileNullCheck(inFile, "writing stacktrace.txt");
        print("\n");

        while(fscanf(inFile, "%c", &fileChar) != EOF)
        {
            printf("%c", fileChar);
        }
        fclose(inFile);
    }
}

//This function saves the instructions that was stored
//mcode.txt to stacktrack.txt
int storeCodeArray()
{
    FILE* inputFile = fopen("mcode.txt", "r");
    vmInstruction *current = (vmInstruction*)malloc(sizeof(vmInstruction));
    int i, fileLine, op, l, m;

    fileNullCheck(inputFile, "reading mcode.txt");
    memoryNullCheck(current);
    //get the first line from the input then test to see if
    fileLine = fscanf(inputFile, "%d%d%d", &op, &l, &m);

    for(i = 0; i < MAX_CODE_LENGTH && fileLine != EOF; i++)
    {
        current->op = op;
        current->l = l;
        current->m = m;
        code[i] = current;
        current = (vmInstruction*)malloc(sizeof(vmInstruction));
        memoryNullCheck(current);
        fileLine = fscanf(inputFile, "%d%d%d", &op, &l, &m);
    }
    fclose(inputFile);
    return i - 1;
}

//Fetch the instructions
void fetch()
{
    //Ensure that the PC instruction does not exceed the max length of code
    if(pc > MAX_CODE_LENGTH)
    {
        ir = NULL;
    }
    else
    {
        ir = code[pc];
    }
    pc = pc + 1;
}

//Execute the instruction and return whether halt conditions are met
int execute(int halt)
{

    //halt condition is met, which means it hit an invalid instruction
    if (halt)
    {
        return halt;
    }

    switch(ir->op)
    {
        case 1://Push value M onto stack
            sp = sp + 1;
            stack[sp] = ir->m;
            break;
        case 2://arithmetic or logical operations
            switch(ir->m)
            {
                case 0://RET
                    sp = bp - 1;
                    pc = stack[sp + 4];
                    bp = stack[sp + 3];
                    //Discard previous activation level
                    activationIndex--;
                    break;
                case 1://neg
                    stack[sp] = -stack[sp];
                    break;
                case 2://add
                    sp = sp - 1;
                    stack[sp] = stack[sp] + stack[sp + 1];
                    break;
                case 3://sub
                    sp = sp - 1;
                    stack[sp] = stack[sp] - stack[sp + 1];
                    break;
                case 4://mul
                    sp = sp - 1;
                    stack[sp] = stack[sp] * stack[sp + 1];
                    break;
                case 5://div
                    sp = sp - 1;
                    stack[sp] = stack[sp] / stack[sp + 1];
                    break;
                case 6://odd
                    stack[sp] = stack[sp] % 2;
                    break;
                case 7://mod
                    sp = sp - 1;
                    stack[sp] = stack[sp] % stack[sp + 1];
                    break;
                case 8://eql
                    sp = sp - 1;
                    stack[sp] = stack[sp] == stack[sp + 1];
                    break;
                case 9://neq
                    sp = sp - 1;
                    stack[sp] = stack[sp] != stack[sp + 1];
                    break;
                case 10://lss
                    sp = sp - 1;
                    stack[sp] = stack[sp] < stack[sp + 1];
                    break;
                case 11://leq
                    sp = sp - 1;
                    stack[sp] = stack[sp] <= stack[sp + 1];
                    break;
                case 12://gtr
                    sp = sp - 1;
                    stack[sp] = stack[sp] > stack[sp + 1];
                    break;
                case 13://geq
                    sp = sp - 1;
                    stack[sp] = stack[sp] >= stack[sp + 1];
                    break;
                default://halt when trying to perform an invalid operation
                    halt = 1;
                    break;
            }
            break;
        case 3://Get value at offset M in frame L levels down and push it
            sp = sp + 1;
            stack[sp] = stack[base(ir->l, bp) + ir->m];
            break;
        case 4://Pop stack and insert value at offset M in frame L levels down
            stack[base(ir->l, bp) + ir->m] = stack[sp];
            sp = sp - 1;
            break;
        case 5://Call procedure at M
            stack[sp + 1] = 0; // return value (FV)
            stack[sp + 2] = base(ir->l, bp); // static link (SL)
            stack[sp + 3] = bp; // dynamic link (DL)
            stack[sp + 4] = pc; // return address (RA)
            bp = sp + 1;
            pc = ir->m;
            //Save activation levels for later
            activationLevel[activationIndex] = bp;
            activationIndex++;
            break;
        case 6://Allocate M locals on stack
            sp = sp + ir->m;
            break;
        case 7://Jump to M
            pc = ir->m;
            break;
        case 8://Pop stack and jump to M if value is equal to 0
            if (stack[sp] == 0)
            {
                pc = ir->m;
            }
            sp = sp - 1;
            break;
        case 9:
            if(ir->m == 0)  //Pop stack and print out value
            {
                print();
                sp = sp - 1;
            }
            else if (ir->m == 1)    //Read in input from user and push it
            {
                sp = sp + 1;
                read();
            }
            else    //Halt the machine
            {
                halt = 1;
            }
            break;
        default:
            //halt when trying to perform an operation that does not exist
            halt = 1;
            break;
    }
    return halt;
}

//Output Code to file stacktrace
void outputCodeToFile(FILE* outputFile, int numberOfinstructions)
{
    int i;

    fprintf(outputFile, "Line\tOP\tL\tM\n");

    for(i = 0; i <= numberOfinstructions; i++ )
    {
        fprintf(outputFile, "%s", getCurrentCode(i));
        fprintf(outputFile, "\n");
    }
}

//Gets the string representation of the current instruction
//Returns a string
char* getCurrentCode(int i)
{
    int l, m;
    char op[4], *currentCode = (char*)calloc(13, sizeof(char));

    //get the text name for the current operation
    strcpy(op, getOperationName(code[i]->op));
    l = code[i]->l;
    m = code[i]->m;
    //create a string of current instruction
    sprintf(currentCode, "%d\t%s\t%d\t%d", i, op, l, m);
    return currentCode;
}

//Saves the current state of the registers and stack
void outputCurrentState(FILE* outputFile)
{
    int i, prevActivation = 0;

    //save the registers
    fprintf(outputFile, "%d\t%d\t%d", pc, bp, sp);
    //save the stack
    for(i = 1; i <= sp; i++)
    {

        if(i == 1)
        {
            fprintf(outputFile, "\t");
        }
        else
        {
            fprintf(outputFile, " ");
        }

        if(printSeperator(prevActivation, i) == 1)
        {
            fprintf(outputFile, "| ");
            prevActivation++;
        }
        fprintf(outputFile, "%d", stack[i]);
    }
    fprintf(outputFile, "\n");
}

//Request integer from the user then push it onto the stack
void read()
{
    int input;
    printf("Enter a value to be pushed onto the stack: ");
    scanf("%d", &input);
    stack[sp] = input;
}

//Prints the current state of the stack to the console
void print()
{
    int i, prevActivation = 0;

    printf("\nStack:\n");

    for(i = 1; i <= sp; i++)
    {

        if(i != 1)
        {
            printf(" ");
        }

        if(printSeperator(prevActivation, i) == 1)
        {
            printf("| ");
            prevActivation++;
        }
        printf("%d", stack[i]);
    }
    printf("\n\n");
}

//This function checks to see if the current instruction ir is valid
//Return 1 if the instruction is not valid
int validateInstruction(int max)
{
    int result = 0;

    if(ir == NULL|| ir->l > MAX_LEXI_LEVELS || pc > max)
    {
        result = 1;
    }
    return result;
}

//This gets the text name of the current operation
char* getOperationName(int op)
{
    char* name;

    switch(op)
    {
        case 1:
            name = "lit";
            break;
        case 2:
            name = "opr";
            break;
        case 3:
            name = "lod";
            break;
        case 4:
            name = "sto";
            break;
        case 5:
            name = "cal";
            break;
        case 6:
            name = "inc";
            break;
        case 7:
            name = "jmp";
            break;
        case 8:
            name = "jpc";
            break;
        case 9:
            name = "sio";
            break;
        default:
            name = "";
            break;
    }
    return name;
}

//Finds where to print "|" activation level separator
int printSeperator(int current, int index)
{
    int i;

    for(i = current; i < activationIndex; i++)
    {
        if(index == activationLevel[i])
        {
            return 1;
        }
    }
    return 0;
}

//Check to see if memory is allocated
void memoryNullCheck(vmInstruction *current)
{

    if(current == NULL)
    {
        printf("Unable to allocate memory for current instruction. Exiting program\n");
        exit(-1);
    }
}

//Check to see if the file is available and open
void fileNullCheck(FILE* inputFile, char* message)
{

    if(inputFile == NULL)
    {
        printf("Error %s file. Exiting program\n", message);
        exit(-1);
    }
}

//This function finds a variable in a different Activation Record L levels down
int base(int level, int b)
{

    while (level > 0)
    {
        b = stack[b + 1];
        level--;
    }
    return b;
}
