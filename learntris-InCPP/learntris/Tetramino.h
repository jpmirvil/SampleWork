#pragma once

#include "stdafx.h"
//lside of tetramino, used rotations
enum Side
{
    top = 0, right, bottom, left
};

//the position of the the active tetramino
typedef struct Position
{
    int row;
    int col;
    int orientation;
}TetraPosition;

class Tetramino
{
    int length;
    int space[4]; //enum Side: 0 = top, 1 = right, 2 = bottom, 3 = left
    TetraPosition position;
    std::vector<std::vector<char>> shape;
    char name = '\0';

public:
    Tetramino();
    void setTetramino(char command);
    std::vector<std::vector<char>> getShape();
    void rotate();
    char getName();
    TetraPosition getPosition();
    void updatePosition(int rowMove, int colMove);
    int *getSpace();
};

