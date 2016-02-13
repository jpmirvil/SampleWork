#include "stdafx.h"

//table to confirm valid symbol, initialized on object construction
static char systable[256];

Game::Game()
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

void Game::updateBoard()
{
    std::string currentLine;

    for (int row = 0; row < 22; row++)
    {
        std::getline(std::cin, currentLine);
        if (currentLine.find_first_not_of(' ') != std::string::npos)
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

std::vector<std::vector<char>> Game::getBoard()
{
    return board;
}

char Game::getValueAt(int row, int col)
{
    return board[row][col];
}

void Game::createBoard()
{
    board.resize(depth, std::vector<char>(width, '.'));
}


void Game::printBoardState(bool printTetramino)
{
    for (int row = 0; row < 22; row++)
    {
        for (int col = 0; col < 10; col++)
        {
            char current = getValueAt(row, col);
            current = printTetramino ? getMovingTetra(row, col, current) : current;
            std::cout << current << ' ';
        }
        std::cout << std::endl;
    }
    if (gameOver)
    {
        std::cout << "Game Over" << std::endl;
    }
}

char Game::getMovingTetra(int row, int col, char current)
{
    int rowDifference = row - activeTetramino.getPosition().row;
    int colDifference = col - activeTetramino.getPosition().col;

    if (current == empty)
    {
        if (rowDifference < (int)activeTetramino.getShape().size() && rowDifference >= 0)
        {
            if (colDifference < (int)activeTetramino.getShape().size() && colDifference >= 0)
            {
                return std::toupper(activeTetramino.getShape()[rowDifference][colDifference]);
            }
        }
    }
    return current;
}

void Game::clearBoard()
{
    board.clear();
    createBoard();
}

void Game::getScore()
{
    std::cout << score << std::endl;
}


void Game::getLinesCleared()
{
    std::cout << linesCleared << std::endl;
}


void Game::step()
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

void Game::setActiveTetramino(char command)
{
    activeTetramino.setTetramino(command);
}

void Game::displayActiveTetramino()
{
    auto shape = activeTetramino.getShape();

    for (size_t row = 0; row < shape.size(); row++)
    {
        for (size_t col = 0; col < shape.size(); col++)
        {
            std::cout << shape[row][col] << ' ';
        }
        std::cout << std::endl;
    }
}

void Game::rotatePiece(char direction)
{
    int rotations = direction == ')' ? 1 : 3;

    while (rotations > 0)
    {
        activeTetramino.rotate();
        --rotations;
    }
}

void Game::movePiece(char direction)
{
    int rowMove = 0;
    int colMove = 0;
    bool hardDrop = false;

    Position tetraPos = activeTetramino.getPosition();
    const int *space = activeTetramino.getSpace();
    int size = (int)activeTetramino.getShape().size();


    if (direction == '<'){ colMove = -1; }
    else if (direction == '>'){ colMove = 1; }
    else if (direction == 'v'){ rowMove = 1; }
    else if (direction == 'V'){ hardDrop = true; }

    //check boundary
    if (colMove != 0 && tetraPos.col + colMove + space[(right + tetraPos.orientation) % 4] >= 0)//left wall
    {
        if (tetraPos.col + colMove - space[(left + tetraPos.orientation) % 4] <= 10 - size) //right wall
        {
            //object on board to the left or right??
            int colToTest = colMove < 0 ? tetraPos.col - 1 + space[(left + tetraPos.orientation) % 4] : tetraPos.col + 1 + space[(right + tetraPos.orientation) % 4];
            int temp = size;
            //begin test

            for (int row = tetraPos.row; row < tetraPos.row + size && (board[row][colToTest] == empty); row++)
            {
                temp--;
            }
            if (temp == 0)
            {
                activeTetramino.updatePosition(0, colMove);
            }
        }
    }

    if (tetraPos.row + rowMove + space[(top + tetraPos.orientation) % 4] >= 0)
    {

        if (tetraPos.row + rowMove - space[(bottom + tetraPos.orientation) % 4] <= 22 - size)
        {
            activeTetramino.updatePosition(rowMove, 0);
        }
    }

    if (hardDrop)
    {
        int mostMoves = 21;
        int rowOffset = size - space[(tetraPos.orientation + bottom) % 4];
        int colOffset = tetraPos.col;

        for (int col = colOffset; col < colOffset + size; col++)
        {
            int startRow = size;

            //find beginning
            for (int i = size - 1; i >= 0 && activeTetramino.getShape()[i][col - colOffset] == empty; i--)
            {
                startRow--;
            }
            int moves = 0;
            for (int row = startRow; row < 22 && (/*startRow == size || */(board[row][col] == empty)); row++)//add or all rows are blank?
            {
                moves++;
            }
            mostMoves = moves < mostMoves ? moves : mostMoves;
        }
        activeTetramino.updatePosition(mostMoves, 0);

        for (int row = mostMoves; row < mostMoves + rowOffset; row++)
        {
            for (int col = colOffset; col < colOffset + size; col++)
            {
                if (col < 10 && activeTetramino.getShape()[row - mostMoves][col - colOffset] != empty)
                {
                    board[row][col] = activeTetramino.getShape()[row - mostMoves][col - colOffset];
                }

            }
        }
        if (activeTetramino.getPosition().row < 2){ gameOver = true; }
    }
}