#include "stdafx.h"

static char systable[256];

GameBoard::GameBoard()
{
	createBoard();
	systable[empty] = '.';
	systable[green] = 'g';
	systable[blue] = 'b';
	systable[cyan] = 'c';
	systable[red] = 'r';
	systable[magenta] = 'm';
	systable[orange] = 'o';
	systable[yellow] = 'y';
};

void GameBoard::updateBoard()
{
	string currentLine;

	for (int row = 0; row < 22; row++)
	{
		std::getline(cin, currentLine);
		int index = 0;

		for (int col = 0; col < currentLine.length(); col++)
		{
			if (systable[currentLine[col]])
			{
				board[row][index] = currentLine[col];
				index++;
			}
		}
	}
}

vector<vector<char>> GameBoard::getBoard()
{ 
	return board; 
}

char GameBoard::getValueAt(int row, int col)
{ 
	return board[row][col]; 
}

void GameBoard::createBoard()
{
	board.resize(depth, vector<char>(width, '.'));
}


void GameBoard::printBoardState()
{
	for (int row = 0; row < 22; row++)
	{

		for (int col = 0; col < 10; col++)
		{
			cout << getValueAt(row, col) << ' ';
		}
		cout << endl;
	}
}

void GameBoard::clearBoard()
{
	board.clear();
	createBoard();
}

void GameBoard::getScore() 
{
	cout << score << endl;
}

void GameBoard::calculateScore()
{
	//Calc score here
}

void GameBoard::getLinesCleared()
{
	cout << linesCleared << endl;
}

void GameBoard::setLinesCleared()
{
	//set lines cleared
}

void GameBoard::step()
{
	for (int i = 0; i < board.size(); i++)
	{
		bool isUnbroken = true;
		auto start = board[i].begin();
		auto end = board[i].end();

		for (auto it = start; it != end; it++)
		{
			if (*it == '.')
			{
				isUnbroken = false;
				break;
			}
		}

		if (isUnbroken)
		{
			for (auto it = start; it != end; it++)
			{
				*it = '.';
			}
			linesCleared++;
			score += 100;
		}
	}
}
