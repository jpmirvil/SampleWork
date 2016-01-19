#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CHARACTER_MAX 11
#define DIGIT_MAX 5

/*
James Mirvil & Atul Thomas
COP 3402
Project 4
PL/0 Compiler
*/

//Struct for a lexeme
typedef struct
{
    int kind;
    char name[12];
} namerecord_t;

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


//Global Variables
char* cleanedInput;
int ssym[256] = {0}; // list of special symbols
int lexemeTableLength = 0;
int commentRemoved;
int printToConsole = 0;
namerecord_t *lexemeTable;

//Functions
int isReservedWord(char *reserved, int length, char *input);
int isSymbol(char character);
char testForComment(FILE* inFile);
char* getVarString(char* str);
void initialize();
void createTokenFromInput();
void outputCleanInput();
void outputLexemeTable();
void outputLexemeList();
void validateLexeme(char * lexeme, int token);
void nullFileCheck(FILE* file, char* fileName);
void nullMemoryCheck(void* variable);

//Entry point for Scanner
void scanner(int printScanner)
{
    //Initialize the valid symbols
    ssym['+'] = plussym;
    ssym['-'] = minussym;
    ssym['*'] = multsym;
    ssym['/'] = slashsym;
    ssym['('] = lparentsym;
    ssym[')'] = rparentsym;
    ssym['='] = eqlsym;
    ssym[','] = commasym;
    ssym['.'] = periodsym;
    ssym['#'] = neqsym;
    ssym['<'] = lessym;
    ssym['>'] = gtrsym;
    ssym[';'] = semicolonsym;
    ssym[':'] = becomessym;

    printToConsole = printScanner;

    //Read input from file
    initialize();
    //Output code without comments
    outputCleanInput();
    //Analyze lexemes and add to array
    createTokenFromInput();
    //Output lexemes
    outputLexemeTable();
    outputLexemeList();
}

//Read all the text from the input file to an array
void initialize()
{
    char currentCharacter, *temp;
    int i = 0;
    FILE* inFile = fopen("input.txt", "r");

    //Initialize size of clean input
    cleanedInput = (char*) malloc(100 * sizeof(char));
    // Ensure file exist before using
    nullFileCheck(inFile, "input.txt");

    //Read input file into an array
    while(fscanf(inFile, "%c", &currentCharacter) != EOF)
    {
        while(currentCharacter == '/')
        {
            //Remove comments from input
            currentCharacter = testForComment(inFile);
            if(commentRemoved == 0)
            {
                cleanedInput[i] = '/';
                i++;
            }
        }
        //Add text to array
        if((i + 2) % 100 == 0)
        {
            //we ran out of allocated space, add more
            temp = (char*)realloc(cleanedInput, (i + 102) * sizeof(char));
            nullMemoryCheck(temp);
            cleanedInput = temp;
        }
        cleanedInput[i] = currentCharacter;
        i++;
    }
    //Add Null terminating character to make it a string
    cleanedInput[i] = '\0';
    fclose(inFile);
}

//Parses the cleaned text into tokens
void createTokenFromInput()
{
    char temp[3], *name;
    int i;
    namerecord_t *memTemp;
    lexemeTable = (namerecord_t*) malloc(50 * sizeof(namerecord_t));

    //DFA
    for(i = 0; i < strlen(cleanedInput); i++)
    {
        //set kind to an invalid case for when reserved words are not found
        lexemeTable[lexemeTableLength].kind = 0;

        //Test if it is an identifier
        switch(cleanedInput[i])
        {
            case ' '://Do not tokenize whitespaces
            case '\t':
            case '\r':
            case '\n':
                lexemeTableLength--;
                break;
            case 'b'://begin
                if(isReservedWord("begin", 5, cleanedInput + i))
                {
                    lexemeTable[lexemeTableLength].kind = beginsym;
                    strcpy(lexemeTable[lexemeTableLength].name, "begin");
                    i += 4;
                }
                break;
            case 'c'://call or const
                //call
                if(isReservedWord("call", 4, cleanedInput + i))
                {
                    lexemeTable[lexemeTableLength].kind = callsym;
                    strcpy(lexemeTable[lexemeTableLength].name, "call");
                    i += 3;
                }
                //const
                else if (isReservedWord("const", 5, cleanedInput + i))
                {
                    lexemeTable[lexemeTableLength].kind = constsym;
                    strcpy(lexemeTable[lexemeTableLength].name, "const");
                    i += 4;
                }
                break;
            case 'd'://do
                if(isReservedWord("do", 2, cleanedInput + i))
                {
                    lexemeTable[lexemeTableLength].kind = dosym;
                    strcpy(lexemeTable[lexemeTableLength].name, "do");
                    i += 1;
                }
                break;
            case 'e'://else or end
                //else
                if(isReservedWord("else", 4, cleanedInput + i))
                {
                    lexemeTable[lexemeTableLength].kind = elsesym;
                    strcpy(lexemeTable[lexemeTableLength].name, "else");
                    i += 3;
                }
                //end
                else if (isReservedWord("end", 3, cleanedInput + i))
                {
                    lexemeTable[lexemeTableLength].kind = endsym;
                    strcpy(lexemeTable[lexemeTableLength].name, "end");
                    i += 2;
                }
                break;
            case 'i'://if
                if(isReservedWord("if", 2, cleanedInput + i))
                {
                    lexemeTable[lexemeTableLength].kind = ifsym;
                    strcpy(lexemeTable[lexemeTableLength].name, "if");
                    i += 1;
                }
                break;
            case 'o'://odd
                if(isReservedWord("odd", 3, cleanedInput + i))
                {
                    lexemeTable[lexemeTableLength].kind = oddsym;
                    strcpy(lexemeTable[lexemeTableLength].name, "odd");
                    i += 2;
                }
                break;
            case 'p'://procedure
                if(isReservedWord("procedure", 9, cleanedInput + i))
                {
                    lexemeTable[lexemeTableLength].kind = procsym;
                    strcpy(lexemeTable[lexemeTableLength].name, "procedure");
                    i += 8;
                }
                break;
            case 'r'://read
                if(isReservedWord("read", 4, cleanedInput + i))
                {
                    lexemeTable[lexemeTableLength].kind = readsym;
                    strcpy(lexemeTable[lexemeTableLength].name, "read");
                    i += 3;
                }
                break;
            case 't'://then
                if(isReservedWord("then", 4, cleanedInput + i))
                {
                    lexemeTable[lexemeTableLength].kind = thensym;
                    strcpy(lexemeTable[lexemeTableLength].name, "then");
                    i += 3;
                }
                break;
            case 'v'://var
                if(isReservedWord("var", 3, cleanedInput + i))
                {
                    lexemeTable[lexemeTableLength].kind = varsym;
                    strcpy(lexemeTable[lexemeTableLength].name, "var");
                    i += 2;
                }
                break;
            case 'w'://while or write
                if(isReservedWord("while", 5, cleanedInput + i))
                {
                    lexemeTable[lexemeTableLength].kind = whilesym;
                    strcpy(lexemeTable[lexemeTableLength].name, "while");
                    i += 4;
                }

                else if (isReservedWord("write", 5, cleanedInput + i))
                {
                    lexemeTable[lexemeTableLength].kind = writesym;
                    strcpy(lexemeTable[lexemeTableLength].name, "write");
                    i += 4;
                }
                break;
            default:
                break;
        }
        //Is not a reserve word, see if an identifier, number or symbol
        if(lexemeTable[lexemeTableLength].kind == 0 && !isspace(cleanedInput[i]))
        {
            //is variable (identifier)
            if(isalpha(cleanedInput[i]))
            {
                lexemeTable[lexemeTableLength].kind = identsym;
                name = getVarString(cleanedInput + i);
                validateLexeme(name, identsym);
                strcpy(lexemeTable[lexemeTableLength].name, name);
                i += strlen(lexemeTable[lexemeTableLength].name) - 1;
            }

            //is a number
            else if(isdigit(cleanedInput[i]))
            {
                lexemeTable[lexemeTableLength].kind = numbersym;
                name = getVarString(cleanedInput + i);
                validateLexeme(name, numbersym);
                strcpy(lexemeTable[lexemeTableLength].name, name);
                i += strlen(lexemeTable[lexemeTableLength].name) - 1;
            }

            //is symbol
            else if(isSymbol(cleanedInput[i]))
            {

                lexemeTable[lexemeTableLength].kind = ssym[(int)cleanedInput[i]];
                temp[0] = cleanedInput[i];
                temp[1] = '\0';
                strcpy(lexemeTable[lexemeTableLength].name, temp);

                //Special Case these: "<>" , "<=" , ">=", and ":="
                if(isSymbol(cleanedInput[i + 1]))
                {
                    switch(cleanedInput[i])
                    {
                        case '<':
                            temp[0] = '<';
                            temp[1] = cleanedInput[i + 1];
                            temp[2] = '\0';
                            strcpy(lexemeTable[lexemeTableLength].name, temp);

                            //not equal to
                            if(cleanedInput[i + 1] == '>')
                            {
                                lexemeTable[lexemeTableLength].kind = neqsym;
                            }

                            // less than or equal to
                            else if(cleanedInput[i + 1] == '=')
                            {
                                lexemeTable[lexemeTableLength].kind = leqsym;
                            }
                            //not a valid sequence of symbols
                            else
                            {
                                validateLexeme(temp, 0);
                            }
                            break;
                        case '>':
                            lexemeTable[lexemeTableLength].kind = geqsym;
                            temp[0] = '>';
                            temp[1] = cleanedInput[i + 1];
                            temp[2] = '\0';
                            strcpy(lexemeTable[lexemeTableLength].name, temp);

                            //not a valid sequence of symbols
                            if(cleanedInput[i + 1] != '=')
                            {
                                validateLexeme(temp, 0);
                            }
                            break;
                        case ':':
                            lexemeTable[lexemeTableLength].kind = ssym[(int)cleanedInput[i]];
                            temp[0] = ':';
                            temp[1] = cleanedInput[i + 1];
                            temp[2] = '\0';
                            strcpy(lexemeTable[lexemeTableLength].name, temp);

                            //not a valid sequence of symbols
                            if(cleanedInput[i + 1] != '=')
                            {
                                validateLexeme(temp, 0);
                            }
                            break;
                        default:
                            i--;
                            break;
                    }
                    i++;
                }
            }
            //An invalid symbol was reached
            else
            {
                validateLexeme(cleanedInput + i, 0);
            }
        }

        //We are near the end of allocated space, reallocate
        if((lexemeTableLength + 1) % 50 == 0)
        {
            memTemp = (namerecord_t*) realloc(lexemeTable, (lexemeTableLength + 51) * sizeof(namerecord_t));
            nullMemoryCheck(memTemp);
            lexemeTable = memTemp;
        }
        lexemeTableLength++;
    }
}

//Check to see is the sequence of chars are is a valid reserved word
int isReservedWord(char *reserved, int length, char *input)
{
    int i, j, isReserved = 1;

    //iterate through the sequence to see if the letter equal
    for( i = 0; i < length && isReserved; i++)
    {
        j = i;

        //If current characters don't match, then it is not reserved
        if(reserved[i] != input[j])
        {
            isReserved = 0;
        }
    }
    //Make sure the current state is reserve
    if(isReserved)
    {
        //Verify that there is whitespace or period so that its not a variable
        if(isspace(input[length]) || isSymbol(input[length]))
        {
            //is a reserved word
            return 1;
        }
    }
    //Not a reserved word
    return 0;
}

//Checks to see if a character is a valid PL/0 symbol
int isSymbol(char character)
{
    if(character < 256 && character >= 0)
    {
        //Only index with a number is a valid symbol
        if(ssym[(int)character] != 0)
        {
            return 1;
        }
    }
    return 0;
}

//Return the char sequence for a identifier / number
char* getVarString(char* str)
{
    int i = 0;
    char * varString;

    //if it alphanumeric then increment index
    while(!isSymbol(str[i]) && !isspace(str[i]))
    {
        i++;
    }
    //create then return the string
    varString = (char*) malloc((i + 1) * sizeof(char));
    nullMemoryCheck(varString);
    strncpy(varString, str, i);
    varString[i] = '\0';
    return varString;
}

//Outputs the input text minus the comments
void outputCleanInput()
{
    FILE* outFile = fopen("cleaninput.txt", "w");

    nullFileCheck(outFile, "cleaninput.txt");
    fprintf(outFile, "%s",cleanedInput);
    fclose(outFile);
}

//Outputs the lexeme table
void outputLexemeTable()
{
    FILE* outFile = fopen("lexemetable.txt", "w");
    int i;

    nullFileCheck(outFile, "lexemetable.txt");
    fprintf(outFile,"lexeme      token type\n");

    //iterate through list and output each value int table
    for(i = 0; i < lexemeTableLength; i++)
    {
        fprintf(outFile, "%-12s%d\n", lexemeTable[i].name, lexemeTable[i].kind);
    }
    fclose(outFile);
}

//Output the lexeme list
void outputLexemeList()
{
    FILE* outFile = fopen("lexemelist.txt", "w");
    int i;

    nullFileCheck(outFile, "lexemelist.txt");

    //print the kind for each value in the list
    for( i = 0; i < lexemeTableLength; i++)
    {
        fprintf(outFile, "%d ", lexemeTable[i].kind);
        if(printToConsole)
        {
            printf("%d ", lexemeTable[i].kind);
        }

        //if it is a variable then print out variable name
        if(lexemeTable[i].kind == identsym )
        {
            fprintf(outFile, "%s ", lexemeTable[i].name);
            if(printToConsole)
            {
                printf("%s ", lexemeTable[i].name);
            }
        }
        //if it is a number then print out number
        else if(lexemeTable[i].kind == numbersym)
        {
            fprintf(outFile, "%s ", lexemeTable[i].name);
            if(printToConsole)
            {
                printf("%s ", lexemeTable[i].name);
            }
        }
    }
    if(printToConsole)
    {
        printf("\n\n");
    }
    fclose(outFile);
}

//ensure that the current lexeme is a valid one
void validateLexeme(char *lexeme, int tokenType)
{
    int i;

    switch(tokenType)
    {
        case identsym://Name too long.
            if(strlen(lexeme) > CHARACTER_MAX)
            {
                printf("Identifier '%s' exceeds the max length of %d. Exiting Program\n", lexeme, CHARACTER_MAX);
                outputLexemeTable();
                outputLexemeList();
                exit(-1);
            }
            break;
        case numbersym: //Number too long.
            if(strlen(lexeme) > DIGIT_MAX)
            {
                printf("Error 25: '%s' This number is too large. Exiting Program\n", lexeme);
                outputLexemeTable();
                outputLexemeList();
                exit(-1);
            }

            else //Variable does not start with letter.
            {
                for (i = 0; i < strlen(lexeme); i++)
                {
                    if (!isdigit(lexeme[i]))
                    {
                        printf("Variable '%c' does not start with a letter. Exiting Program\n", lexeme[i]);
                        outputLexemeTable();
                        outputLexemeList();
                        exit(-1);
                    }
                }
            }
            break;
        case 0: //Invalid symbols.
            printf("Invalid symbol '%c' used. Exiting Program\n", lexeme[0]);
            outputLexemeTable();
            outputLexemeList();
            exit(-1);
            break;
        default:
            break;
    }
}

//Checks to see if the current sequence is a comment
//If there is a comment, it is removed
char testForComment(FILE* inFile)
{
    char currentCharacter, commentText[3] = {0};
    int location = 1;

    fscanf(inFile, "%c", &currentCharacter);
    commentRemoved = 0;

    if(currentCharacter == '*')
    {
        //Throw away input until end of comment is found
        while(strcmp(commentText, "*/") != 0 && location != EOF)
        {
            //shift string to left to test for end of comment found
            commentText[0] = commentText[1];
            location = fscanf(inFile, "%c", &currentCharacter);
            commentText[1] = currentCharacter;

            //Test for nested comments
            if(currentCharacter == '/' &&  commentText[0] != '*')
            {
                commentText[1] = testForComment(inFile);
            }
        }
        commentRemoved = 1;
        //Set to a space if we have reached EOF
        currentCharacter = ' ';
        //Return the character after the comment
        return currentCharacter;
    }
    //Char is not the beginning of comment
    return currentCharacter;
}

//Test to see if file is valid and able to be used
void nullFileCheck(FILE* file, char* fileName)
{
    if(file == NULL)
    {
        printf("Error accessing %s. Exiting program\n", fileName);
        exit(-1);
    }
}

//Test to see if memory was allocated and is now able to be used
void nullMemoryCheck(void* variable)
{
    if(variable == NULL)
    {
        printf("Unable to allocate memory. Exiting program\n");
        exit(-1);
    }
}
