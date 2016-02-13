#pragma once

#include "stdafx.h"

//name of valid board cells
enum BlockSym {
    empty = '.', blue = 'b', cyan = 'c', green = 'g',
    magenta = 'm', orange = 'o', red = 'r', yellow = 'y'
};

class Game
{
    // The matrix is 10 cells wide and 22 cells deep
    bool gameOver = false;
    const int depth = 22;
    const int width = 10;
    std::vector<std::vector<char>> board;
    char systable[256];
    int score = 0;
    int linesCleared = 0;
    Tetramino activeTetramino = Tetramino();

public:
    Game();
    void updateBoard();
    std::vector<std::vector<char>> getBoard();
    char getValueAt(int row, int col);
    void createBoard();
    void clearBoard();
    void printBoardState(bool printTetramino);
    void getScore();
    void getLinesCleared();
    void step();
    void setActiveTetramino(char tetramino);
    void displayActiveTetramino();
    void rotatePiece(char direction);
    void movePiece(char direction);
    char getMovingTetra(int row, int col, char current);
};

