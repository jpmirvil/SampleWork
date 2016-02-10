#pragma once

#include "stdafx.h"

typedef struct location
{
    int row;
    int col;
}tetraLocation;

class tetramino
{
    int length;
    tetraLocation position;// = { 0, 21 };
    vector<vector<char>> shape;
    char name = '\0';

public:
    tetramino();
    void setTetramino(char command);
    vector<vector<char>> getShape();
    void rotate();
    char getName();
    tetraLocation getPosition();
};

