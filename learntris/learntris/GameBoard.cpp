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
        if (currentLine.find_first_not_of(' ') != string::npos)
        {
            int index = 0;

            for (size_t col = 0; col < currentLine.length(); col++)
            {
                if (systable[currentLine[col]])
                {
                    board[row][index] = currentLine[col];
                    index++;
                }
            }
        }
        else
        {
            row--;
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


void GameBoard::printBoardState(bool printTetramino)
{
	for (int row = 0; row < 22; row++)
	{
		for (int col = 0; col < 10; col++)
		{
            char current = getValueAt(row, col);
            current = printTetramino ? getStuff(row, col, current) : current;
            cout << current << ' ';
		}
		cout << endl;
	}
}

char GameBoard::getStuff(int row, int col, char current)
{
    int rowDifference = row - activeTetramino.getPosition().row;
    int colDifference = col - activeTetramino.getPosition().col;

    if (rowDifference < (int) activeTetramino.getShape().size() && rowDifference >= 0)
    {
        if (colDifference < (int) activeTetramino.getShape().size() && colDifference >= 0)
        {
            return std::toupper(activeTetramino.getShape()[rowDifference][colDifference]);
        }
    }
    return current;
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
    for (size_t i = 0; i < board.size(); i++)
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

void GameBoard::setActiveTetramino(char command)
{
    activeTetramino.setTetramino(command);
    //activeTetramino = tetramino(command);
}

void GameBoard::displayActiveTetramino()
{
    auto shape = activeTetramino.getShape();

    for (size_t row = 0; row < shape.size(); row++)
    {
        for (size_t col = 0; col < shape.size(); col++)
        {
            cout << shape[row][col] << ' ';
        }
        cout << endl;
    }
}

void GameBoard::rotatePieceRight()
{
    activeTetramino.rotate();
}
