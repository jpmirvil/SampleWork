#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOL_TABLE_SIZE 100
#define MAX_CODE_LENGTH 500
/*
James Mirvil & Atul Thomas
COP 3402
Project 4
PL/0 Compiler
*/

typedef struct symbol
{
    int kind; // const = 1, var = 2, proc = 3
    char name[12]; // name up to 11 chars
    int val; // number (ASCII value)
    int level; // L level
    int addr; // M address
} symbol;

typedef struct mcode
{
    int op; // number (ASCII value)
    int l; // L level
    int m; // M address
} mcode;

//enums for valid symbols
typedef enum
{
    nulsym = 1, identsym, numbersym, plussym, minussym,
    multsym, slashsym, oddsym, eqlsym, neqsym, lessym, leqsym,
    gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
    periodsym, becomessym, beginsym, endsym, ifsym, thensym,
    whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
    readsym , elsesym
} token_type;

//enums for OP Codes
typedef enum
{
    LIT = 1, OPR, LOD, STO, CAL, INC, JMP, JPC, SIO
} opCodes;

//enums for Operations
typedef enum
{
    RET = 0, NEG, ADD, SUB, MUL, DIV, ODD, MOD,
    EQL, NEQ, LSS, LEQ, GTR, GEQ
} oprCodes;

typedef enum
{
    constant = 1, variable, procedure
} identifierType;

//Global Variables
symbol *tokenStream;
symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
symbol *TOKEN;
mcode genCode[MAX_CODE_LENGTH];
int tokenStreamLength = 0;
int symbolTableSize = 0;
int codeIndex = 0;
int tokenIndex = 0;
int currentLevel = -1;
char tokenTable[34][13];
char errorTable[26][55];

//Functions
void createTokenStream();
void getNextToken(symbol *TOKEN);
void program();
void statement();
void condition();
void expression();
void term();
void factor();
void error(int errorCode);
void initializeTokenTable();
void enter(int kind, char* name, int level, int address);
void emit(int op, int l, int m);
void outputGeneratedCode(int printParcer);
void nullFileCheck(FILE* file, char* fileName);
void nullMemoryCheck(void* variable);
int block();
int relation(int kind);
int find(char* indent);
int symbolType(int index);
int symbolLevel(int index);
int symbolAddress(int index);
int symbolValue(int index);

//entry point for the parser / code generator
void parser(int printParser)
{
    initializeTokenTable();
    createTokenStream();
    TOKEN = NULL;
    program();
    outputGeneratedCode(printParser);
}

//Read in scanner file then create token stream
void createTokenStream()
{
    int i = 0, currentToken, numberValue;
    char indentifier[12];
    FILE* inFile = fopen("lexemelist.txt", "r");

    nullFileCheck(inFile, "lexemelist.txt");
    tokenStream = (symbol*) malloc(100 * sizeof(symbol));
    symbol *memTemp;

    while(fscanf(inFile, "%d", &currentToken) != EOF)
    {
        tokenStream[i].kind = currentToken;

        if(currentToken == identsym)
        {
            if(fscanf(inFile, "%s", indentifier) != EOF)
            {
                strcpy(tokenStream[i].name, indentifier);
            }
        }
        else if(currentToken == numbersym)
        {
            if(fscanf(inFile, "%d", &numberValue) != EOF)
            {
                tokenStream[i].val = numberValue;
                strcpy(tokenStream[i].name, tokenTable[currentToken]);
            }
        }
        else
        {
            strcpy(tokenStream[i].name, tokenTable[currentToken]);
        }
        i++;

        if((i + 1) % 100 == 0)
        {
            memTemp = (symbol*) realloc(tokenStream, (i + 101) * sizeof(symbol));
            nullMemoryCheck(memTemp);
            tokenStream = memTemp;
        }
    }
    fclose(inFile);
    tokenStreamLength = i;
}

//Main entry for the program
void program()
{

    int programAddress;
    //Get first token
    getNextToken(TOKEN);
    programAddress = block();
    if(programAddress == -1);

    if(TOKEN->kind != periodsym)
    {
        error(9);
    }
    //clear out final RET and replace it with Halt
    if(genCode[codeIndex].op == RET)
    {
        codeIndex--;
    }
    emit(SIO, 0, 2);
    printf("No errors, program is syntactically correct.\n");
}

//Retrieve next token from stream
void getNextToken(symbol *current)
{
    //not first token, proceed to next
    if(current != NULL)
    {
        tokenIndex++;
        TOKEN = &tokenStream[tokenIndex];
    }
    //if its null, then we are at the first token
    else
    {
        TOKEN = &tokenStream[0];
    }
}

//Block entry point
int block()
{
    //allocated space for registers
    int preAllocated = 4, jmpAddress = codeIndex, prevSX;
    int procAddress, i;
    char identifier[13];
    emit(JMP, 0, codeIndex);
    currentLevel++;
    prevSX = symbolTableSize;

    if(TOKEN->kind == constsym)
    {
        //For constants, store kind, name and value.
        do
        {
            getNextToken(TOKEN);

            if(TOKEN->kind != identsym)
            {
                error(4);
            }
            strcpy(identifier, TOKEN->name);
            getNextToken(TOKEN);

            if(TOKEN->kind != eqlsym)
            {
                error(3);
            }
            getNextToken(TOKEN);

            if(TOKEN->kind != numbersym)
            {
                error(2);
            }
            enter(constant, identifier, currentLevel, TOKEN->val);
            getNextToken(TOKEN);
        }
        while(TOKEN->kind == commasym);

        if(TOKEN->kind != semicolonsym)
        {
            error(5);
        }
        getNextToken(TOKEN);
    }

    if(TOKEN->kind == varsym)
    {
        //For variables, store kind, name, L and M.
        do
        {
            getNextToken(TOKEN);

            if(TOKEN->kind != identsym)
            {
                error(4);
            }
            strcpy(identifier, TOKEN->name);
            getNextToken(TOKEN);
            enter(variable, identifier, currentLevel, preAllocated);
            preAllocated++;
        }
        while (TOKEN->kind == commasym);

        if(TOKEN->kind != semicolonsym)
        {
            error(5);
        }
        getNextToken(TOKEN);
    }

    while (TOKEN->kind == procsym)
    {
        //For procedures, store kind, name, L and M.
        getNextToken(TOKEN);

        if(TOKEN->kind != identsym)
        {
            error(4);//6
        }
        strcpy(identifier, TOKEN->name);
        enter(procedure, TOKEN->name, currentLevel, codeIndex + 1);
        getNextToken(TOKEN);

        if(TOKEN->kind != semicolonsym)
        {
            error(5);
        }
        getNextToken(TOKEN);
        procAddress = block();

        i = find(identifier);
        if(i == -1)
        {
            error(11);
        }
        symbol_table[i].addr = procAddress;

        if(TOKEN->kind != semicolonsym)
        {
            error(5);
        }
        getNextToken(TOKEN);
    }
    //change jump location to next instruction
    genCode[jmpAddress].m = codeIndex;
    //allocate space
    emit(INC, 0, preAllocated);
    procAddress = codeIndex - 1;
    statement();
    //RET call
    emit(OPR, 0, 0);
    symbolTableSize = prevSX;
    currentLevel--;
    return procAddress;
}

//Statement entry point
void statement()
{
    int i, codeTemp, prevCodeIndex, jumpIndex, jumpAddress;

    if(TOKEN->kind == identsym)
    {
        i = find(TOKEN->name);
        if(i == -1)
        {
            error(11);
        }
        if(symbolType(i) != variable)
        {
            error(12);
        }
        getNextToken(TOKEN);

        if(TOKEN->kind != becomessym)
        {
            error(1);
        }
        getNextToken(TOKEN);
        expression();
        emit(STO, currentLevel - symbolLevel(i), symbolAddress(i));
    }

    else if(TOKEN->kind == callsym)
    {
        getNextToken(TOKEN);

        if(TOKEN->kind != identsym)
        {
            error(14);
        }
        i = find(TOKEN->name);
        if(i == -1)
        {
            error(11);
        }
        if(symbolType(i) != procedure)
        {
            error(15);
        }
        emit(CAL, currentLevel - symbolLevel(i), symbolAddress(i));
        getNextToken(TOKEN);
    }
    else if(TOKEN->kind == beginsym)
    {
        getNextToken(TOKEN);
        statement();

        while(TOKEN->kind == semicolonsym)
        {
            getNextToken(TOKEN);
            statement();
        }

        if(TOKEN->kind != endsym)
        {
            error(17);
        }
        getNextToken(TOKEN);
    }
    else if(TOKEN->kind == ifsym)
    {
        getNextToken(TOKEN);
        condition();

        if(TOKEN->kind != thensym)
        {
            error(16);
        }
        getNextToken(TOKEN);
        codeTemp = codeIndex;
        emit(JPC, 0, 0);
        statement();

        if(TOKEN->kind == elsesym)
        {
            jumpAddress = codeIndex;
            emit(JMP, 0, 0);
            genCode[codeTemp].m = codeIndex;
            getNextToken(TOKEN);
            statement();
            genCode[jumpAddress].m = codeIndex;
        }
        else
        {
            genCode[codeTemp].m = codeIndex;
        }
    }
    else if(TOKEN->kind == whilesym)
    {
        prevCodeIndex = codeIndex;
        getNextToken(TOKEN);
        condition();
        jumpIndex = codeIndex;
        emit(JPC, 0, 0);

        if(TOKEN->kind != dosym)
        {
            error(18);
        }
        getNextToken(TOKEN);
        statement();
        emit(JMP, 0, prevCodeIndex);
        genCode[jumpIndex].m = codeIndex;
    }
    else if(TOKEN->kind == readsym)
    {
        getNextToken(TOKEN);

        if(TOKEN->kind != identsym)
        {
            error(19);
        }

        i = find(TOKEN->name);
        if(i == -1)
        {
            error(11);
        }
        emit(SIO, 0, 1);
        emit(STO, currentLevel - symbolLevel(i), symbolAddress(i));
        getNextToken(TOKEN);
    }
    else if(TOKEN->kind == writesym)
    {
        getNextToken(TOKEN);

        if(TOKEN->kind != identsym)
        {
            error(19);
        }

        i = find(TOKEN->name);
        if(i == -1)
        {
            error(11);
        }
        emit(LOD, currentLevel - symbolLevel(i), symbolAddress(i));
        emit(SIO, 0, 0);
        getNextToken(TOKEN);
    }
}

//Condition Entry point
void condition()
{
    int operation;

    if(TOKEN->kind == oddsym)
    {
        getNextToken(TOKEN);
        expression();
        emit(OPR, 0, ODD);
    }
    else
    {
        expression();

        if(relation(TOKEN->kind) != 1)
        {
            error(20);
        }
        operation = TOKEN->kind;
        getNextToken(TOKEN);
        expression();

        //=, <>, <=,>=, <, >
        switch(operation)
        {
        case eqlsym:
            emit(OPR, 0, EQL);
            break;
        case neqsym:
            emit(OPR, 0, NEQ);
            break;
        case leqsym:
            emit(OPR, 0, LEQ);
            break;
        case geqsym:
            emit(OPR, 0, GEQ);
            break;
        case gtrsym:
            emit(OPR, 0, GTR);
            break;
        case lessym:
            emit(OPR, 0, LSS);
            break;
        default:
            break;
        }
    }
}

//Expression entry point
void expression()
{
    int addop;

    if(TOKEN->kind == plussym || TOKEN->kind == minussym)
    {
        addop = TOKEN->kind;
        getNextToken(TOKEN);
        term();

        if(addop == minussym)
        {
            emit(OPR, 0, NEG);
        }
    }
    else
    {
        term();
    }
    while (TOKEN->kind == plussym || TOKEN->kind == minussym)
    {
        addop = TOKEN->kind;
        getNextToken(TOKEN);
        term();

        if(addop == plussym)
        {
            emit(OPR, 0, ADD);
        }
        else
        {
            emit(OPR, 0, SUB);
        }
    }
}

//term entry point
void term()
{
    int mulop;
    factor();

    while(TOKEN->kind == multsym || TOKEN->kind == slashsym)
    {
        mulop = TOKEN->kind;
        getNextToken(TOKEN);
        factor();

        if(mulop == multsym)
        {
            emit(OPR, 0, MUL);
        }
        else
        {
            emit(OPR, 0, DIV);
        }
    }
}

//Factor entry point
void factor()
{
    int i;

    if(TOKEN->kind == identsym)
    {
        i = find(TOKEN->name);

        if(i == -1)
        {
            error(11);
        }
        if(symbolType(i) == variable)
        {
            emit(LOD, currentLevel - symbolLevel(i), symbolAddress(i));
        }
        else if(symbolType(i) == constant)
        {
            emit(LIT, 0, symbolValue(i));
        }
        else
        {
            error(21);
        }
        getNextToken(TOKEN);
    }
    else if(TOKEN->kind == numbersym)
    {
        emit(LIT, 0, TOKEN->val);
        getNextToken(TOKEN);
    }

    else if(TOKEN->kind == lparentsym)
    {
        getNextToken(TOKEN);
        expression();

        if(TOKEN->kind != rparentsym)
        {
            error(22);
        }
        getNextToken(TOKEN);
    }
    else
    {
        error(23);
    }
}

//relation entry point
int relation(int symbol)
{
    //<relation> ::= “=” | “<>” | “<” | “>” | “<=” | “>=” .
    int isRelation;

    switch(symbol)
    {
    case eqlsym:
    case neqsym:
    case lessym:
    case leqsym:
    case geqsym:
    case gtrsym:
        isRelation = 1;
        break;
    default:
        isRelation = 0;
        break;
    }
    return isRelation;
}

//error codes
void error(int errorCode)
{
    //Initialize error table
    strcpy(errorTable[1] , "Use = instead of :=.\n");
    strcpy(errorTable[2] , "= must be followed by a number.\n");
    strcpy(errorTable[3] , "Identifier must be followed by =.\n");
    strcpy(errorTable[4] , "const, var, procedure must be followed by identifier.\n");
    strcpy(errorTable[5] , "Semicolon or comma missing.\n");
    strcpy(errorTable[6] , "Incorrect symbol after procedure declaration.\n");
    strcpy(errorTable[7] , "Statement expected.\n");
    strcpy(errorTable[8] , "Incorrect symbol after statement part in block.\n");
    strcpy(errorTable[9] , "Period expected.\n");
    strcpy(errorTable[10] , "Semicolon between statements missing.\n");
    strcpy(errorTable[11] , "Undeclared identifier.\n");
    strcpy(errorTable[12] , "Assignment to constant or procedure is not allowed.\n");
    strcpy(errorTable[13] , "Assignment operator expected.\n");
    strcpy(errorTable[14] , "Call must be followed by an identifier.\n");
    strcpy(errorTable[15] , "Call of a constant or variable is meaningless.\n");
    strcpy(errorTable[16] , "then expected.\n");
    strcpy(errorTable[17] , "Semicolon or } expected.\n");
    strcpy(errorTable[18] , "do expected.\n");
    strcpy(errorTable[19] , "Incorrect symbol following statement.\n");
    strcpy(errorTable[20] , "Relational operator expected.\n");
    strcpy(errorTable[21] , "Expression must not contain a procedure identifier.\n");
    strcpy(errorTable[22] , "Right parenthesis missing.\n");
    strcpy(errorTable[23] , "The preceding factor cannot begin with this symbol.\n");
    strcpy(errorTable[24] , "An expression cannot begin with this symbol.\n");
    strcpy(errorTable[25] , "This number is too large.\n");

    //Output current code
    outputGeneratedCode(0);
    //print error then stop program
    printf("Error number %d, %s, TOKEN: %s", errorCode, errorTable[errorCode], TOKEN->name);
    exit(-1);
}

//initialize token table
void initializeTokenTable()
{
    strcpy(tokenTable[0], "");
    strcpy(tokenTable[1], "nulsym");
    strcpy(tokenTable[2], "identsym");
    strcpy(tokenTable[3], "numbersym");
    strcpy(tokenTable[4], "plussym");
    strcpy(tokenTable[5], "minussym");
    strcpy(tokenTable[6], "multsym");
    strcpy(tokenTable[7], "slashsym");
    strcpy(tokenTable[8], "oddsym");
    strcpy(tokenTable[9], "eqlsym");
    strcpy(tokenTable[10], "neqsym");
    strcpy(tokenTable[11], "lessym");
    strcpy(tokenTable[12], "leqsym");
    strcpy(tokenTable[13], "gtrsym");
    strcpy(tokenTable[14], "geqsym");
    strcpy(tokenTable[15], "lparentsym");
    strcpy(tokenTable[16], "rparentsym");
    strcpy(tokenTable[17], "commasym");
    strcpy(tokenTable[18], "semicolonsym");
    strcpy(tokenTable[19], "periodsym");
    strcpy(tokenTable[20], "becomessym");
    strcpy(tokenTable[21], "beginsym");
    strcpy(tokenTable[22], "endsym");
    strcpy(tokenTable[23], "ifsym");
    strcpy(tokenTable[24], "thensym");
    strcpy(tokenTable[25], "whilesym");
    strcpy(tokenTable[26], "dosym");
    strcpy(tokenTable[27], "callsym");
    strcpy(tokenTable[28], "constsym");
    strcpy(tokenTable[29], "varsym");
    strcpy(tokenTable[30], "procsym");
    strcpy(tokenTable[31], "writesym");
    strcpy(tokenTable[32], "readsym");
    strcpy(tokenTable[33], "elsesym");
}

//enter entry point
void enter(int kind, char* name, int level, int address)
{
    strcpy(symbol_table[symbolTableSize].name, name);
    symbol_table[symbolTableSize].kind = kind;
    //constant
    if (kind == constant)
    {
        symbol_table[symbolTableSize].val = address;
    }
    // variable & procedure
    else
    {
        symbol_table[symbolTableSize].level = level;
        symbol_table[symbolTableSize].addr = address;
    }
    symbolTableSize++;
}

//emit entry point
void emit(int op, int l, int m)
{
    if(codeIndex > MAX_SYMBOL_TABLE_SIZE)
    {
        error(25);
    }
    else
    {
        genCode[codeIndex].op = op;
        genCode[codeIndex].l = l;
        genCode[codeIndex].m = m;
        codeIndex++;
    }
}

//Find a identifier in the symbols table
int find(char* indent)
{
    int i;

    for(i = symbolTableSize - 1; i >= 0; i--)
    {
        if(strcmp(indent, symbol_table[i].name) == 0)
        {
            return i;
        }
    }
    return -1;
}

//gets the type of a specified symbol
int symbolType(int index)
{
    return symbol_table[index].kind;
}

//gets the level of a specified symbol
int symbolLevel(int index)
{
    return symbol_table[index].level;
}

//gets the address of a specified symbol
int symbolAddress(int index)
{
    return symbol_table[index].addr;
}

//gets the value of a specified symbol
int symbolValue(int index)
{
    return symbol_table[index].val;
}

//Generate the output code
void outputGeneratedCode(int outputToConsole)
{
    int i;
    FILE* outFile = fopen("mcode.txt", "w");
    nullFileCheck(outFile, "mcode.txt");

    if(outputToConsole)
    {
        printf("\nOP L M\n");
    }

    for(i = 0; i < codeIndex; i++)
    {
        fprintf(outFile, "%d %d %d\n", genCode[i].op, genCode[i].l, genCode[i].m);
        if(outputToConsole)
        {
            printf("%d %d %d\n", genCode[i].op, genCode[i].l, genCode[i].m);
        }
    }
    if(outputToConsole)
    {
        printf("\n");
    }
    fclose(outFile);
}

