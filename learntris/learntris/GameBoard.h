#pragma once

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

enum blockSym {
	empty = '.', blue = 'b', cyan = 'c', green = 'g', 
	magenta = 'm', orange = 'o', red = 'r', yellow = 'y' 
};

class GameBoard
{
	// The matrix is 10 cells wide and 22 cells deep,
	const int depth = 22;
	const int width = 10;
	vector<vector<char>> board;
	char systable[256];
	int score = 0;
	int linesCleared = 0;

public:
	GameBoard();
	void updateBoard();
	vector<vector<char>> getBoard();
	char getValueAt(int row, int col);
	void createBoard();
	void clearBoard();
	void printBoardState();
	void getScore();
	void calculateScore();
	void getLinesCleared();
	void setLinesCleared();
	void step();
};

