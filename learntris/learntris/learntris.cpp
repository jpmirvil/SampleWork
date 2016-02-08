// learntris.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

int main(int argc, char* argv[])
{
	bool gameOver = false;
	GameBoard test = GameBoard();

	while (!gameOver)
	{
		string command;
		std::getline(cin, command);

		switch (command[0])
		{
		case 'c':
			test.clearBoard();
			break;
		case 'g':
			test.updateBoard();
			break;
		case 'q':
			gameOver = true;
			break;
		case 'p':
			test.printBoardState();
			break;
		case 's':
			test.step();
			break;
		case '?':	
			if (command[1] == 's')
			{
				test.getScore();
			}
			else if (command[1] == 'n')
			{
				test.getLinesCleared();
			}
			break;
		default:
			break;
		}
	}
	return 0;
}