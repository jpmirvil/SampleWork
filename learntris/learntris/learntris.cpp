// learntris.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

int main(int argc, char* argv[])
{
	bool gameOver = false;
	GameBoard board = GameBoard();

    ////int test[3][3] = { { 1, 2, 3 }, {4,5,6}, {7,8,9} };
    //int test[4][4] = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, {13,14,15,16} };

    //int length = 4;
    //for (int row = 0; row < length / 2; row++)
    //{
    //    for (int col = row; col < length - row - 1; col++)
    //    {
    //        int temp = test[row][col];
    //        //start swaps
    //        test[row][col] = test[length - col - 1][row];
    //        test[length - col - 1][row] = test[length - row - 1][length - col - 1];
    //        test[length - row - 1][length - col - 1] = test[col][length - row - 1];
    //        test[col][length - row - 1] = temp;
    //    }
    //}

    //for (auto outter : test)
    //{
    //    for (int i = 0; i < length; i++)
    //    {
    //        cout << outter[i] << ' ';
    //    }
    //    cout << endl;
    //}

	while (!gameOver)
	{
		string command;
		//std::getline(cin, command);
        cin >> command;

		switch (command[0])
		{
		case 'c':
			board.clearBoard();
			break;
		case 'g':
			board.updateBoard();
			break;
		case 'q':
			gameOver = true;
			break;
        case 'P':
		case 'p':
            board.printBoardState(std::isupper(command[0]));
			break;
		case 's':
			board.step();
			break;
        case 't':
            board.displayActiveTetramino();
            break;
        case 'I':
        case 'J':
        case 'L':
        case 'O':
        case 'S':
        case 'T':
        case 'Z':
            board.setActiveTetramino(command[0]);
            break;
        case ')':
            board.rotatePieceRight();
            break;
        case ';':
            cout << endl;
            break;
		case '?':	
			if (command[1] == 's')
			{
				board.getScore();
			}
			else if (command[1] == 'n')
			{
				board.getLinesCleared();
			}
			break;
		default:
			break;
		}
	}
    //system("pause");
	return 0;
}