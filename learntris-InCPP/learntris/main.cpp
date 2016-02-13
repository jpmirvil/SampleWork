// learntris.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

void enterPausedState();
void enterLegalScreen();

int main(int argc, char* argv[])
{
    bool gameOver = false;
    Game session = Game();

    while (!gameOver)
    {
        char command;
        std::cin >> command;
        switch (command)
        {
            case 'c':
                session.clearBoard();
                break;
            case 'g':
                session.updateBoard();
                break;
            case 'q':
                gameOver = true;
                break;
            case 'P':
            case 'p':
                session.printBoardState(std::isupper(command) != 0);
                break;
            case 's':
                session.step();
                break;
            case 't':
                session.displayActiveTetramino();
                break;
            case 'I':
            case 'J':
            case 'L':
            case 'O':
            case 'S':
            case 'T':
            case 'Z':
                session.setActiveTetramino(command);
                break;
            case ')':
            case '(':
                session.rotatePiece(command);
                break;
            case '<':
            case '>':
            case 'v':
            case 'V':
                session.movePiece(command);
                break;
            case ';':
                std::cout << std::endl;
                break;
            case '?':
                char nextCommand;
                std::cin >> nextCommand;
                if (nextCommand == 's')
                {
                    session.getScore();
                }
                else if (nextCommand == 'n')
                {
                    session.getLinesCleared();
                }
                break;
            case '!':
                enterPausedState();
                break;
            case '@':
                enterLegalScreen();
                break;
            default:
                break;
        }
    }
    return 0;
}

void enterLegalScreen()
{
    bool leaveScreen = false;

    while (!leaveScreen)
    {
        char command;
        std::cin >> command;

        if (command == '!')
        {
            leaveScreen = true;
        }
        else if (command == 'p')
        {
            std::cout << "Learntris (c) 1992 Tetraminex, Inc." << std::endl;
            std::cout << "Press start button to begin." << std::endl;
        }
    }
}

void enterPausedState()
{
    bool paused = true;
    std::cout << "Paused" << std::endl;
    std::cout << "Press start button to continue." << std::endl;

    while (paused)
    {
        char command;
        std::cin >> command;

        if (command == '!')
        {
            paused = false;
        }
    }
}