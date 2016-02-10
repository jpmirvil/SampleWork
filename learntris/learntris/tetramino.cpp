#include "stdafx.h"

tetramino::tetramino()
{
    //default constructor
};

void tetramino::setTetramino(char command)
{
    name = command;
    shape.clear();
    position.row = 0;
 
    switch (name)
    {
        case 'I':
            length = 4;
            shape.resize(length, vector<char>(length, '.'));
            shape[1] = { 'c', 'c', 'c', 'c' };
            position.col = 2;
            break;
        case 'J':
            length = 3;
            shape.resize(length, vector<char>(length, '.'));
            shape[0] = { 'b', '.', '.' };
            shape[1] = { 'b', 'b', 'b' };
            position.col = 3;
            break;
        case 'L':
            length = 3;
            shape.resize(length, vector<char>(length, '.'));
            shape[0] = { '.', '.', 'o' };
            shape[1] = { 'o', 'o', 'o' };
            position.col = 3;
            break;
        case 'O':
            length = 2;
            shape.resize(length, vector<char>(length, 'y'));
            position.col = 4;
            break;
        case 'S':
            length = 3;
            shape.resize(length, vector<char>(length, '.'));
            shape[0] = { '.', 'g', 'g' };
            shape[1] = { 'g', 'g', '.' };
            position.col = 3;
            break;
        case 'T':
            length = 3;
            shape.resize(length, vector<char>(length, '.'));
            shape[0] = { '.', 'm', '.' };
            shape[1] = { 'm', 'm', 'm' };
            position.col = 3;
            break;
        case 'Z':
            length = 3;
            shape.resize(length, vector<char>(length, '.'));
            shape[0] = { 'r', 'r', '.' };
            shape[1] = { '.', 'r', 'r' };
            position.col = 3;
            break;
        default:
            length = 4;
            shape.resize(length, vector<char>(length, '.'));
            break;
    }
};

void tetramino::rotate()
{

    for (int row = 0; row < length / 2; row++)
    {
        for (int col = row; col < length - row - 1; col++)
        {
            int temp = shape[row][col];
            //start swaps
            shape[row][col] = shape[length - col - 1][row];
            shape[length - col - 1][row] = shape[length - row - 1][length - col - 1];
            shape[length - row - 1][length - col - 1] = shape[col][length - row - 1];
            shape[col][length - row - 1] = temp;
        }
    }
}

tetraLocation tetramino::getPosition()
{
    return position;
}

vector<vector<char>> tetramino::getShape()
{
    return shape;
}

char tetramino::getName()
{
    return name;
}