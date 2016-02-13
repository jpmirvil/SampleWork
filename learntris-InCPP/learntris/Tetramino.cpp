#include "stdafx.h"

Tetramino::Tetramino()
{
    //default constructor
};

void Tetramino::setTetramino(char command)
{
    name = command;
    shape.clear();
    space[0] = space[1] = space[2] = space[3] = 0;
    position.row = 0;
    position.orientation = 0;

	//set properties of tetramino
    switch (name)
    {
        case 'I':
            length = 4;
            shape.resize(length, std::vector<char>(length, '.'));
            shape[1] = { 'c', 'c', 'c', 'c' };
            position.col = 3;
            space[0] = 1;
            space[bottom] = 2;
            break;
        case 'J':
            length = 3;
            shape.resize(length, std::vector<char>(length, '.'));
            shape[0] = { 'b', '.', '.' };
            shape[1] = { 'b', 'b', 'b' };
            position.col = 3;
            space[bottom] = 1;
            break;
        case 'L':
            length = 3;
            shape.resize(length, std::vector<char>(length, '.'));
            shape[0] = { '.', '.', 'o' };
            shape[1] = { 'o', 'o', 'o' };
            position.col = 3;
            space[bottom] = 1;
            break;
        case 'O':
            length = 2;
            shape.resize(length, std::vector<char>(length, 'y'));
            position.col = 4;
            break;
        case 'S':
            length = 3;
            shape.resize(length, std::vector<char>(length, '.'));
            shape[0] = { '.', 'g', 'g' };
            shape[1] = { 'g', 'g', '.' };
            position.col = 3;
            space[bottom] = 1;
            break;
        case 'T':
            length = 3;
            shape.resize(length, std::vector<char>(length, '.'));
            shape[0] = { '.', 'm', '.' };
            shape[1] = { 'm', 'm', 'm' };
            position.col = 3;
            space[bottom] = 1;
            break;
        case 'Z':
            length = 3;
            shape.resize(length, std::vector<char>(length, '.'));
            shape[0] = { 'r', 'r', '.' };
            shape[1] = { '.', 'r', 'r' };
            position.col = 3;
            space[bottom] = 1;
            break;
        default:
            length = 4;
            shape.resize(length, std::vector<char>(length, '.'));
            break;
    }
};

void Tetramino::rotate()
{
	//rotate by shifting
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
    position.orientation = (position.orientation + 1) % 4;
}

TetraPosition Tetramino::getPosition()
{
    return position;
}

void Tetramino::updatePosition(int rowMove, int colMove)
{
    position.col += colMove;
    position.row += rowMove;
}

std::vector<std::vector<char>> Tetramino::getShape()
{
    return shape;
}

char Tetramino::getName()
{
    return name;
}
int *Tetramino::getSpace()
{
    return space;
}