// created by: Jay Woo
// course: CS382
// date: October 22, 2017

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <iostream>

using namespace std;

/*
	TTTBoard: class for TTTBoard
	Contains necessary functions and variables to use minmax algortihm
	for 3-D tic-tac-toe.
*/
class TTTBoard
{
	public:
	// current state of entire board
	int boardState[4][4][4];
	
	// checks to see if four in a row has been made
	int winDetector();

	// copy the state of a board
	void copyBoard(int copiedFrom[4][4][4]); 

	// determines if the AI is x or o
	void whoIsAI();
	
	// caluclates the minmax value of a state
	void calcMinMax(int currentPly, int maxPly);

	// evauluates how good a state is 
	void scoreEvaluator();

	// contructor
	TTTBoard();

	// prints out the current state of the board
	void printBoard();

	// variabless
	int score;
	bool isAIo;
	bool isAIx;
	bool AITurn;
	int layer;
	int row;
	int column;
	bool firstValueSet;
	
	// tree
	TTTBoard* children[4][4][4];
	TTTBoard* parent;
};


struct Move {
	int x;
	int y;
	int z;
};

/*
	TTTBoard: constructor for TTTBoard
	args: none
	returns: void
	Defaults values for TTTBoard,
*/
TTTBoard::TTTBoard()
{
	// default values
	firstValueSet = false;
	score = 0;
	parent = NULL;

	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			for( int k = 0; k < 4; k++ )
			{
				children[i][j][k] = NULL;
			}
		}
	}
}

/*
	printBoard: displays board
	args: none
	returns: void
	Prints out current state of board.
*/
void TTTBoard::printBoard()
{
	// show start of printing board
	cout << "board start" << endl;

	// loop through entire board
	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			for( int k = 0; k < 4; k++ )
			{
				// print out board
				if(boardState[i][j][k] == -1)
					cout << 'x';

				else if(boardState[i][j][k] == 0)
					cout << '.';

				else if(boardState[i][j][k] == 1)
					cout << 'o';
			}

			// new row
			cout << endl;
		}

		// new layer
		cout << endl;
	}

	// show end if printing board
	cout << "board end" << endl;
}

/*
	scoreEvaluator: determines how could the state of the board is
	args: none
	returns: void
	Uses the hueristic of my possible wins - opponents possible wins.
*/
void TTTBoard::scoreEvaluator()
{
	// local variables
	int counter;
	int secondCounter;
	bool rowCounted = false;
	bool columnCounted[4] = {false, false, false, false};
	bool leftDiagonalVertical = false;
	bool rightDiagonalVertical = false;
	bool bottomRightDiagonalVertical = false;
	bool bottomLeftDiagonalVertical = false;
	bool leftHorizontalDiagonal = false;
	bool rightHorizontalDiagonal = false;
	bool leftDiagonaLayer[4] = {false, false, false, false};
	bool rightDiagonaLayer[4] = {false, false, false, false};
	bool straightVertical[4][4] = {{false, false, false, false}, {false, false, false, false}, {false, false, false, false}, {false, false, false, false}};
	bool diagonalUpDown[4] = {false, false, false, false};
	bool diagonalDownUp[4] = {false, false, false, false};
	int possibleXWins = 0;
	int possibleOWins = 0;
	int i, j, k;
	

////////////////////////////////////////////////////////////////////////////Determine score for x's

	// loop through entire board
	for( i = 0; i < 4; i++ )
	{
		// reset double counting for columns
		for(counter = 0; counter < 4; counter++)
			columnCounted[counter] = false;
	
		for( j = 0; j < 4; j++ )
		{
			// reset double counting for rows
			rowCounted = false;

			for( k = 0; k < 4; k++ )
			{
				// find an x
				if(boardState[i][j][k] == -1)
				{
					// diagonal vertical from first column
					if(((i == 0 && j == 0 && k == 0) || (i == 1 && j == 1 && k == 1) || (i == 2 && j == 2 && k == 2) || (i == 3 && j == 3 && k == 3)) && 
						leftDiagonalVertical == false)
					{
						// check if way to win is possible
						if(boardState[0][0][0] != 1 && boardState[1][1][1] != 1 && boardState[2][2][2] != 1 && boardState[3][3][3] != 1)
						{
							leftDiagonalVertical = true;
							possibleXWins++;
						}
					}

					// diagonal vertical from last column
					if(((i == 0 && j == 0 && k == 3) || (i == 1 && j == 1 && k == 2) || (i == 2 && j == 2 && k == 1) || (i == 3 && j == 3 && k == 0)) && 
						rightDiagonalVertical == false)
					{
						// check if way to win is possible
						if(boardState[0][0][3] != 1 && boardState[1][1][2] != 1 && boardState[2][2][1] != 1 && boardState[3][3][0] != 1)
						{
							rightDiagonalVertical = true;
							possibleXWins++;
						}
					}

					// diagonal vertical from bottom right edge
					if(((i == 0 && j == 3 && k == 3) || (i == 1 && j == 2 && k == 2) || (i == 2 && j == 1 && k == 1) || (i == 3 && j == 0 && k == 0)) && 
						bottomRightDiagonalVertical == false)
					{
						// check if way to win is possible
						if(boardState[0][3][3] != 1 && boardState[1][2][2] != 1 && boardState[2][1][1] != 1 && boardState[3][0][0] != 1)
						{
							bottomRightDiagonalVertical = true;
							possibleXWins++;
						}
					}

					// diagonal vertical from bottom left edge
					if(((i == 0 && j == 3 && k == 0) || (i == 1 && j == 2 && k == 1) || (i == 2 && j == 1 && k == 2) || (i == 3 && j == 0 && k == 3)) && 
						bottomLeftDiagonalVertical == false)
					{
						// check if way to win is possible
						if(boardState[0][3][0] != 1 && boardState[1][2][1] != 1 && boardState[2][1][2] != 1 && boardState[3][0][3] != 1)
						{
							bottomLeftDiagonalVertical = true;
							possibleXWins++;
						}
					}

					// each layers column
					for(counter = 0; counter < 4; counter++)
					{
						// check if way to win is possible
						if(boardState[i][counter][k] == 1 || 
						  (columnCounted[0] && k == 0) || (columnCounted[1] && k == 1) || 
						  (columnCounted[2] && k == 2) || (columnCounted[3] && k == 3))
							break;

						if(counter == 3)
						{
							// set column to true so it won't be double counted later on
							if(k == 0)
								columnCounted[0] = true;

							else if(k == 1)
								columnCounted[1] = true;
	
							else if(k == 2)
								columnCounted[2] = true;
	
							else if(k == 3)
								columnCounted[3] = true;

							possibleXWins++;
						}
					}


					// each layers row
					for(counter = 0; counter < 4; counter++)
					{
						// check if way to win is possible
						if(boardState[i][j][counter] == 1 || rowCounted == true)
							break;

						if(counter == 3)
						{
							rowCounted = true;
							possibleXWins++;
						}
					}
				

					// each layers top left diagonal
					if((j == 0 && k == 0) || (j == 1 && k == 1) || (j == 2 && k == 2) || (j == 3 && k == 3))
					{
						for(counter = 0, secondCounter = 0; counter < 4; counter++, secondCounter++)
						{
							// check if way to win is possible
							if(boardState[i][counter][secondCounter] == 1 || (leftDiagonaLayer[0] && i == 0) || (leftDiagonaLayer[1] && i == 1) || 
							  (leftDiagonaLayer[2] && i == 2) || (leftDiagonaLayer[3] && i == 3))
								break;

							if(counter == 3)
							{
								// set diagonal from top left to true so it won't be double counted later on
								if(i == 0)
									leftDiagonaLayer[0] = true;

								else if(i == 1)
									leftDiagonaLayer[1] = true;
			
								else if(i == 2)
									leftDiagonaLayer[2] = true;
			
								else if(i == 3)
									leftDiagonaLayer[3] = true;

								possibleXWins++;
							}
						}
					}

					// each layers top right diagonal
					if((j == 0 && k == 3) || (j == 1 && k == 2) || (j == 2 && k == 1) || (j == 3 && k == 0))
					{
						for(counter = 0, secondCounter = 3; counter < 4; counter++, secondCounter--)
						{
							// check if way to win is possible
							if(boardState[i][counter][secondCounter] == 1 || (rightDiagonaLayer[0] && i == 0) || (rightDiagonaLayer[1] && i == 1) || 
							  (rightDiagonaLayer[2] && i == 2) || (rightDiagonaLayer[3] && i == 3))
								break;

							if(counter == 3)
							{
								// set diagonal from top right to true so it won't be double counted later on
								if(i == 0)
									rightDiagonaLayer[0] = true;

								else if(i == 1)
									rightDiagonaLayer[1] = true;
			
								else if(i == 2)
									rightDiagonaLayer[2] = true;
			
								else if(i == 3)
									rightDiagonaLayer[3] = true;

								possibleXWins++;
							}
						}
					}

					// each layers vertical
					for(counter = 0; counter < 4; counter++)
					{
						// check if way to win is possible
						if(boardState[counter][j][k] == 1 || 
						  (straightVertical[0][0] && j == 0 && k == 0) || (straightVertical[0][1] && j == 0 && k == 1) || 
						  (straightVertical[0][2] && j == 0 && k == 2) || (straightVertical[0][3] && j == 0 && k == 3) || 
						  (straightVertical[1][0] && j == 1 && k == 0) || (straightVertical[1][1] && j == 1 && k == 1) || 
						  (straightVertical[1][2] && j == 1 && k == 2) || (straightVertical[1][3] && j == 1 && k == 3) || 
						  (straightVertical[2][0] && j == 2 && k == 0) || (straightVertical[2][1] && j == 2 && k == 1) || 
						  (straightVertical[2][2] && j == 2 && k == 2) || (straightVertical[2][3] && j == 2 && k == 3) || 
						  (straightVertical[3][0] && j == 3 && k == 0) || (straightVertical[3][1] && j == 3 && k == 1) || 
						  (straightVertical[3][2] && j == 3 && k == 2) || (straightVertical[3][3] && j == 3 && k == 3))
							break;

						if(counter == 3)
						{
							// set straight vertical to true so it won't be double counted later on
							if(j == 0 && k == 0)
								straightVertical[0][0] = true;
	
							else if(j == 0 && k == 1)
								straightVertical[0][1] = true;

							else if(j == 0 && k == 2)
								straightVertical[0][2] = true;

							else if(j == 0 && k == 3)
								straightVertical[0][3] = true;

							else if(j == 1 && k == 0)
								straightVertical[1][0] = true;

							else if(j == 1 && k == 1)
								straightVertical[1][1] = true;

							else if(j == 1 && k == 2)
								straightVertical[1][2] = true;

							else if(j == 1 && k == 3)
								straightVertical[1][3] = true;

							else if(j == 2 && k == 0)
								straightVertical[2][0] = true;

							else if(j == 2 && k == 1)
								straightVertical[2][1] = true;

							else if(j == 2 && k == 2)
								straightVertical[2][2] = true;

							else if(j == 2 && k == 3)
								straightVertical[2][3] = true;

							else if(j == 3 && k == 0)
								straightVertical[3][0] = true;

							else if(j == 3 && k == 1)
								straightVertical[3][1] = true;

							else if(j == 3 && k == 2)
								straightVertical[3][2] = true;

							else if(j == 3 && k == 3)
								straightVertical[3][3] = true;

							possibleXWins++;
						}
					}

					
					// diagonal up/down from first row first layer
					if(((i == 0 && j == 0 && k == 0) || (i == 1 && j == 1 && k == 0) || (i == 2 && j == 2 && k == 0) || (i == 3 && j == 3 && k == 0)) && 
						diagonalUpDown[0] == false)
					{
						// check if way to win is possible
						if(boardState[0][0][0] != 1 && boardState[1][1][0] != 1 && boardState[2][2][0] != 1 && boardState[3][3][0] != 1)
						{
							diagonalUpDown[0] = true;
							possibleXWins++;
						}
					}

					// diagonal up/down from second row second layer 
					if(((i == 0 && j == 0 && k == 1) || (i == 1 && j == 1 && k == 1) || (i == 2 && j == 2 && k == 1) || (i == 3 && j == 3 && k == 1)) && 
						diagonalUpDown[1] == false)
					{
						// check if way to win is possible
						if(boardState[0][0][1] != 1 && boardState[1][1][1] != 1 && boardState[2][2][1] != 1 && boardState[3][3][1] != 1)
						{
							diagonalUpDown[1] = true;
							possibleXWins++;
						}
					}

					// diagonal up/down from third row third layer
					if(((i == 0 && j == 0 && k == 2) || (i == 1 && j == 1 && k == 2) || (i == 2 && j == 2 && k == 2) || (i == 3 && j == 3 && k == 2)) && 
						diagonalUpDown[2] == false)
					{
						// check if way to win is possible
						if(boardState[0][0][2] != 1 && boardState[1][1][2] != 1 && boardState[2][2][2] != 1 && boardState[3][3][2] != 1)
						{
							diagonalUpDown[2] = true;
							possibleXWins++;
						}
					}

					// diagonal up/down from fourth row fourth layer
					if(((i == 0 && j == 0 && k == 3) || (i == 1 && j == 1 && k == 3) || (i == 2 && j == 2 && k == 3) || (i == 3 && j == 3 && k == 3)) && 
						diagonalUpDown[3] == false)
					{
						// check if way to win is possible
						if(boardState[0][0][3] != 1 && boardState[1][1][3] != 1 && boardState[2][2][3] != 1 && boardState[3][3][3] != 1)
						{
							diagonalUpDown[3] = true;
							possibleXWins++;
						}
					}

					// diagonal up/down from fourth row first column first layer
					if(((i == 0 && j == 3 && k == 0) || (i == 1 && j == 2 && k == 0) || (i == 2 && j == 1 && k == 0) || (i == 3 && j == 0 && k == 0)) && 
						diagonalDownUp[0] == false)
					{
						// check if way to win is possible
						if(boardState[0][3][0] != 1 && boardState[1][2][0] != 1 && boardState[2][1][0] != 1 && boardState[3][0][0] != 1)
						{
							diagonalDownUp[0] = true;
							possibleXWins++;
						}
					}

					// diagonal up/down from fourth row second column first layer
					if(((i == 0 && j == 3 && k == 1) || (i == 1 && j == 2 && k == 1) || (i == 2 && j == 1 && k == 1) || (i == 3 && j == 0 && k == 1)) && 
						diagonalDownUp[1] == false)
					{
						// check if way to win is possible
						if(boardState[0][3][1] != 1 && boardState[1][2][1] != 1 && boardState[2][1][1] != 1 && boardState[3][0][1] != 1)
						{
							diagonalDownUp[1] = true;
							possibleXWins++;
						}
					}

					// diagonal up/down from fourth row third column first layer
					if(((i == 0 && j == 3 && k == 2) || (i == 1 && j == 2 && k == 2) || (i == 2 && j == 1 && k == 2) || (i == 3 && j == 0 && k == 2)) && 
						diagonalDownUp[2] == false)
					{
						// check if way to win is possible
						if(boardState[0][3][2] != 1 && boardState[1][2][2] != 1 && boardState[2][1][2] != 1 && boardState[3][0][2] != 1)
						{
							diagonalDownUp[2] = true;
							possibleXWins++;
						}
					}

					// diagonal up/down from fourth row fourth column first layer
					if(((i == 0 && j == 3 && k == 3) || (i == 1 && j == 2 && k == 3) || (i == 2 && j == 1 && k == 3) || (i == 3 && j == 0 && k == 3)) && 
						diagonalDownUp[3] == false)
					{
						// check if way to win is possible
						if(boardState[0][3][3] != 1 && boardState[1][2][3] != 1 && boardState[2][1][3] != 1 && boardState[3][0][3] != 1)
						{
							diagonalDownUp[3] = true;
							possibleXWins++;
						}
					}

					// parallel horizontal from the left
					if((i == 0 && k == 0) || (i == 1 && k == 1) || (i == 2 && k == 2) || (i == 3 && k == 3))
					{
						for(counter = 0, secondCounter = 0; counter < 4; counter++, secondCounter++)
						{
							// check if way to win is possible
							if(boardState[counter][j][secondCounter] != 1 && boardState[counter][j][secondCounter] != 1 && 
							   boardState[counter][j][secondCounter] != 1 && boardState[counter][j][secondCounter] != 1 && leftHorizontalDiagonal == false)
							{
								leftHorizontalDiagonal = true;
								possibleXWins++;	
							}
						}
					}


					// parallel horizontal from the right
					if((i == 0 && k == 3) || (i == 1 && k == 2) || (i == 2 && k == 1) || (i == 3 && k == 0))
					{
						for(counter = 0, secondCounter = 3; counter < 4; counter++, secondCounter--)
						{
							// check if way to win is possible
							if(boardState[counter][j][secondCounter] != 1 && boardState[counter][j][secondCounter] != 1 && 
							   boardState[counter][j][secondCounter] != 1 && boardState[counter][j][secondCounter] != 1 && rightHorizontalDiagonal == false)
							{
								rightHorizontalDiagonal = true;
								possibleXWins++;	
							}
						}
					}
				}
			}
		}
	}

	// reset all double counts
	rowCounted = false;
	leftDiagonalVertical = false;
	rightDiagonalVertical = false;
	bottomLeftDiagonalVertical = false;
	bottomRightDiagonalVertical = false;
	leftHorizontalDiagonal = false;
	rightHorizontalDiagonal = false;

	for(counter = 0; counter < 4; counter++)
	{
		columnCounted[counter] = false;
		leftDiagonaLayer[counter] = false;
		rightDiagonaLayer[counter] = false;
		diagonalUpDown[counter] = false;
		diagonalDownUp[counter] = false;
	}

	for(counter = 0; counter < 4; counter++)
	{
		for(secondCounter = 0; secondCounter < 4; secondCounter++)
			straightVertical[counter][secondCounter] = false;
	}

/*----------------------------------------------------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////Determine score for o's

	// loop through entire board
	for( i = 0; i < 4; i++ )
	{
		// reset double counting for columns
		for(counter = 0; counter < 4; counter++)
			columnCounted[counter] = false;
	
		for( j = 0; j < 4; j++ )
		{
			// reset double counting for columns
			rowCounted = false;

			for( k = 0; k < 4; k++ )
			{
				if(boardState[i][j][k] == 1)
				{
					// diagonal vertical from first column
					if(((i == 0 && j == 0 && k == 0) || (i == 1 && j == 1 && k == 1) || (i == 2 && j == 2 && k == 2) || (i == 3 && j == 3 && k == 3)) && 
						leftDiagonalVertical == false)
					{
						// check if way to win is possible
						if(boardState[0][0][0] != -1 && boardState[1][1][1] != -1 && boardState[2][2][2] != -1 && boardState[3][3][3] != -1)
						{
							leftDiagonalVertical = true;
							possibleOWins++;
						}
					}

					// diagonal vertical from last column
					if(((i == 0 && j == 0 && k == 3) || (i == 1 && j == 1 && k == 2) || (i == 2 && j == 2 && k == 1) || (i == 3 && j == 3 && k == 0)) && 
						rightDiagonalVertical == false)
					{
						// check if way to win is possible
						if(boardState[0][0][3] != -1 && boardState[1][1][2] != -1 && boardState[2][2][1] != -1 && boardState[3][3][0] != -1)
						{
							rightDiagonalVertical = true;
							possibleOWins++;
						}
					}

					// diagonal vertical from bottom right edge
					if(((i == 0 && j == 3 && k == 3) || (i == 1 && j == 2 && k == 2) || (i == 2 && j == 1 && k == 1) || (i == 3 && j == 0 && k == 0)) && 
						bottomRightDiagonalVertical == false)
					{
						// check if way to win is possible
						if(boardState[0][3][3] != 1 && boardState[1][2][2] != 1 && boardState[2][1][1] != 1 && boardState[3][0][0] != 1)
						{
							bottomRightDiagonalVertical = true;
							possibleOWins++;
						}
					}

					// diagonal vertical from bottom left edge
					if(((i == 0 && j == 3 && k == 0) || (i == 1 && j == 2 && k == 1) || (i == 2 && j == 1 && k == 2) || (i == 3 && j == 0 && k == 3)) && 
						bottomLeftDiagonalVertical == false)
					{
						// check if way to win is possible
						if(boardState[0][3][0] != 1 && boardState[1][2][1] != 1 && boardState[2][1][2] != 1 && boardState[3][0][3] != 1)
						{
							bottomLeftDiagonalVertical = true;
							possibleOWins++;
						}
					}

					// each layers column
					for(counter = 0; counter < 4; counter++)
					{
						// check if way to win is possible
						if(boardState[i][counter][k] == -1 || 
						  (columnCounted[0] && k == 0) || (columnCounted[1] && k == 1) || 
						  (columnCounted[2] && k == 2) || (columnCounted[3] && k == 3))
							break;

						if(counter == 3)
						{
							// set column to true so it won't be double counted later on
							if(k == 0)
								columnCounted[0] = true;

							else if(k == 1)
								columnCounted[1] = true;
		
							else if(k == 2)
								columnCounted[2] = true;
		
							else if(k == 3)
								columnCounted[3] = true;

							possibleOWins++;
						}
					}

					// each layers row
					for(counter = 0; counter < 4; counter++)
					{
						// check if way to win is possible
						if(boardState[i][j][counter] == -1 || rowCounted == true)
							break;

						if(counter == 3)
						{
							rowCounted = true;
							possibleOWins++;
						}
					}
				

					// each layers top left diagonal
					if((j == 0 && k == 0) || (j == 1 && k == 1) || (j == 2 && k == 2) || (j == 3 && k == 3))
					{
						for(counter = 0, secondCounter = 0; counter < 4; counter++, secondCounter++)
						{
							// check if way to win is possible
							if(boardState[i][counter][secondCounter] == -1 || (leftDiagonaLayer[0] && i == 0) || (leftDiagonaLayer[1] && i == 1) || 
							  (leftDiagonaLayer[2] && i == 2) || (leftDiagonaLayer[3] && i == 3))
								break;

							if(counter == 3)
							{
								// set diagonal from top left to true so it won't be double counted later on
								if(i == 0)
									leftDiagonaLayer[0] = true;

								else if(i == 1)
									leftDiagonaLayer[1] = true;
			
								else if(i == 2)
									leftDiagonaLayer[2] = true;
			
								else if(i == 3)
									leftDiagonaLayer[3] = true;

								possibleOWins++;
							}
						}
					}

					// each layers top right diagonal
					if((j == 0 && k == 3) || (j == 1 && k == 2) || (j == 2 && k == 1) || (j == 3 && k == 0))
					{
						for(counter = 0, secondCounter = 3; counter < 4; counter++, secondCounter--)
						{
							// check if way to win is possible
							if(boardState[i][counter][secondCounter] == -1 || (rightDiagonaLayer[0] && i == 0) || (rightDiagonaLayer[1] && i == 1) || 
							  (rightDiagonaLayer[2] && i == 2) || (rightDiagonaLayer[3] && i == 3))
								break;

							if(counter == 3)
							{
								// set diagonal from top right to true so it won't be double counted later on
								if(i == 0)
									rightDiagonaLayer[0] = true;

								else if(i == 1)
									rightDiagonaLayer[1] = true;
			
								else if(i == 2)
									rightDiagonaLayer[2] = true;
			
								else if(i == 3)
									rightDiagonaLayer[3] = true;

								possibleOWins++;
							}
						}
					}

					// each layers vertical
					for(counter = 0; counter < 4; counter++)
					{
						// check if way to win is possible
						if(boardState[counter][j][k] == -1 || 
						  (straightVertical[0][0] && j == 0 && k == 0) || (straightVertical[0][1] && j == 0 && k == 1) || 
						  (straightVertical[0][2] && j == 0 && k == 2) || (straightVertical[0][3] && j == 0 && k == 3) || 
						  (straightVertical[1][0] && j == 1 && k == 0) || (straightVertical[1][1] && j == 1 && k == 1) || 
						  (straightVertical[1][2] && j == 1 && k == 2) || (straightVertical[1][3] && j == 1 && k == 3) || 
						  (straightVertical[2][0] && j == 2 && k == 0) || (straightVertical[2][1] && j == 2 && k == 1) || 
						  (straightVertical[2][2] && j == 2 && k == 2) || (straightVertical[2][3] && j == 2 && k == 3) || 
						  (straightVertical[3][0] && j == 3 && k == 0) || (straightVertical[3][1] && j == 3 && k == 1) || 
						  (straightVertical[3][2] && j == 3 && k == 2) || (straightVertical[3][3] && j == 3 && k == 3))
							break;

						if(counter == 3)
						{
							// set straight vertcail to true so it won't be double counted later on
							if(j == 0 && k == 0)
								straightVertical[0][0] = true;
	
							else if(j == 0 && k == 1)
								straightVertical[0][1] = true;

							else if(j == 0 && k == 2)
								straightVertical[0][2] = true;

							else if(j == 0 && k == 3)
								straightVertical[0][3] = true;

							else if(j == 1 && k == 0)
								straightVertical[1][0] = true;

							else if(j == 1 && k == 1)
								straightVertical[1][1] = true;

							else if(j == 1 && k == 2)
								straightVertical[1][2] = true;

							else if(j == 1 && k == 3)
								straightVertical[1][3] = true;

							else if(j == 2 && k == 0)
								straightVertical[2][0] = true;

							else if(j == 2 && k == 1)
								straightVertical[2][1] = true;

							else if(j == 2 && k == 2)
								straightVertical[2][2] = true;

							else if(j == 2 && k == 3)
								straightVertical[2][3] = true;

							else if(j == 3 && k == 0)
								straightVertical[3][0] = true;

							else if(j == 3 && k == 1)
								straightVertical[3][1] = true;

							else if(j == 3 && k == 2)
								straightVertical[3][2] = true;

							else if(j == 3 && k == 3)
								straightVertical[3][3] = true;

							possibleOWins++;
						}
					}

					
					// diagonal up/down from first column
					if(((i == 0 && j == 0 && k == 0) || (i == 1 && j == 1 && k == 0) || (i == 2 && j == 2 && k == 0) || (i == 3 && j == 3 && k == 0)) && 
						diagonalUpDown[0] == false)
					{
						// check if way to win is possible
						if(boardState[0][0][0] != -1 && boardState[1][1][0] != -1 && boardState[2][2][0] != -1 && boardState[3][3][0] != -1)
						{
							diagonalUpDown[0] = true;
							possibleOWins++;
						}
					}

					// diagonal up/down from second column
					if(((i == 0 && j == 0 && k == 1) || (i == 1 && j == 1 && k == 1) || (i == 2 && j == 2 && k == 1) || (i == 3 && j == 3 && k == 1)) && 
						diagonalUpDown[1] == false)
					{
						// check if way to win is possible
						if(boardState[0][0][1] != -1 && boardState[1][1][1] != -1 && boardState[2][2][1] != -1 && boardState[3][3][1] != -1)
						{
							diagonalUpDown[1] = true;
							possibleOWins++;
						}
					}

					// diagonal up/down from third column
					if(((i == 0 && j == 0 && k == 2) || (i == 1 && j == 1 && k == 2) || (i == 2 && j == 2 && k == 2) || (i == 3 && j == 3 && k == 2)) && 
						diagonalUpDown[2] == false)
					{
						// check if way to win is possible
						if(boardState[0][0][2] != -1 && boardState[1][1][2] != -1 && boardState[2][2][2] != -1 && boardState[3][3][2] != -1)
						{
							diagonalUpDown[2] = true;
							possibleOWins++;
						}
					}

					// diagonal up/down from fourth column
					if(((i == 0 && j == 0 && k == 3) || (i == 1 && j == 1 && k == 3) || (i == 2 && j == 2 && k == 3) || (i == 3 && j == 3 && k == 3)) && 
						diagonalUpDown[3] == false)
					{
						// check if way to win is possible
						if(boardState[0][0][3] != -1 && boardState[1][1][3] != -1 && boardState[2][2][3] != -1 && boardState[3][3][3] != -1)
						{
							diagonalUpDown[3] = true;
							possibleOWins++;
						}
					}

					// diagonal up/down from fourth row first column first layer
					if(((i == 0 && j == 3 && k == 0) || (i == 1 && j == 2 && k == 0) || (i == 2 && j == 1 && k == 0) || (i == 3 && j == 0 && k == 0)) && 
						diagonalDownUp[0] == false)
					{
						// check if way to win is possible
						if(boardState[0][3][0] != -1 && boardState[1][2][0] != -1 && boardState[2][1][0] != -1 && boardState[3][0][0] != -1)
						{
							diagonalDownUp[0] = true;
							possibleOWins++;
						}
					}

					// diagonal up/down from fourth row second column first layer
					if(((i == 0 && j == 3 && k == 1) || (i == 1 && j == 2 && k == 1) || (i == 2 && j == 1 && k == 1) || (i == 3 && j == 0 && k == 1)) && 
						diagonalDownUp[1] == false)
					{
						// check if way to win is possible
						if(boardState[0][3][1] != -1 && boardState[1][2][1] != -1 && boardState[2][1][1] != -1 && boardState[3][0][1] != -1)
						{
							diagonalDownUp[1] = true;
							possibleOWins++;
						}
					}

					// diagonal up/down from fourth row third column first layer
					if(((i == 0 && j == 3 && k == 2) || (i == 1 && j == 2 && k == 2) || (i == 2 && j == 1 && k == 2) || (i == 3 && j == 0 && k == 2)) && 
						diagonalDownUp[2] == false)
					{
						// check if way to win is possible
						if(boardState[0][3][2] != -1 && boardState[1][2][2] != -1 && boardState[2][1][2] != -1 && boardState[3][0][2] != -1)
						{
							diagonalDownUp[2] = true;
							possibleOWins++;
						}
					}

					// diagonal up/down from fourth row fourth column first layer
					if(((i == 0 && j == 3 && k == 3) || (i == 1 && j == 2 && k == 3) || (i == 2 && j == 1 && k == 3) || (i == 3 && j == 0 && k == 3)) && 
						diagonalDownUp[3] == false)
					{
						// check if way to win is possible
						if(boardState[0][3][3] != -1 && boardState[1][2][3] != -1 && boardState[2][1][3] != -1 && boardState[3][0][3] != -1)
						{
							diagonalDownUp[3] = true;
							possibleOWins++;
						}
					}

					// parallel horizontal from the left
					if((i == 0 && k == 0) || (i == 1 && k == 1) || (i == 2 && k == 2) || (i == 3 && k == 3))
					{
						for(counter = 0, secondCounter = 0; counter < 4; counter++, secondCounter++)
						{
							// check if way to win is possible
							if(boardState[counter][j][secondCounter] != -1 && boardState[counter][j][secondCounter] != -1 && 
							   boardState[counter][j][secondCounter] != -1 && boardState[counter][j][secondCounter] != -1 && leftHorizontalDiagonal == false)
							{
								leftHorizontalDiagonal = true;
								possibleOWins++;	
							}
						}
					}

					// parallel horizontal from the right
					if((i == 0 && k == 3) || (i == 1 && k == 2) || (i == 2 && k == 1) || (i == 3 && k == 0))
					{
						for(counter = 0, secondCounter = 3; counter < 4; counter++, secondCounter--)
						{
							// check if way to win is possible
							if(boardState[counter][j][secondCounter] != 1 && boardState[counter][j][secondCounter] != 1 && 
							   boardState[counter][j][secondCounter] != 1 && boardState[counter][j][secondCounter] != 1 && rightHorizontalDiagonal == false)
							{
								rightHorizontalDiagonal = true;
								possibleOWins++;	
							}
						}
					}
				}
			}
		}
	}
	
	// return hueristic value depending on if AI is x or o
	if(isAIo == true)
		score = possibleOWins - possibleXWins;

	else if(isAIx == true)
		score = possibleXWins - possibleOWins;
}

/*
	copyBoard: copies board
	args: board to copy from
	returns: void
	Copies board state into current class TTTBoard board.
*/
void TTTBoard::copyBoard(int copiedFrom[4][4][4])
{
	// loop through entire board
	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			for( int k = 0; k < 4; k++ )
			{
				// copy values
				boardState[i][j][k] = copiedFrom[i][j][k];
			}
		}
	}
}

/*
	winDetector: checks if someone has won
	args: none
	returns: int 999 if AI wins or -999 if oppnent wins
	Checks to see if x or o has won.
	Note: there is 76 different ways to win.
	Side note: Yes I hardcoded all the values but I left it this way intentionally so
	that I could easily see the patterns each set of wins presents.
*/
int TTTBoard::winDetector()
{
	// board values go by layer, row, column


////////////////////////////////////////////////////////////Wins for x's


////////////////////////////////////////////////////////////First column of each layer

	// first layer first column
	if(boardState[0][0][0] == -1 && boardState[0][1][0] == -1 && boardState[0][2][0] == -1 && boardState[0][3][0] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// second layer first column
	else if(boardState[1][0][0] == -1 && boardState[1][1][0] == -1 && boardState[1][2][0] == -1 && boardState[1][3][0] == -1)
	{

		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// third layer first column
	else if(boardState[2][0][0] == -1 && boardState[2][1][0] == -1 && boardState[2][2][0] == -1 && boardState[2][3][0] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// fourth layer first column
	else if(boardState[3][0][0] == -1 && boardState[3][1][0] == -1 && boardState[3][2][0] == -1 && boardState[3][3][0] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}


////////////////////////////////////////////////////////////Second column of each layer

	// first layer second column
	else if(boardState[0][0][1] == -1 && boardState[0][1][1] == -1 && boardState[0][2][1] == -1 && boardState[0][3][1] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// second layer second column
	else if(boardState[1][0][1] == -1 && boardState[1][1][1] == -1 && boardState[1][2][1] == -1 && boardState[1][3][1] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// third layer second column
	else if(boardState[2][0][1] == -1 && boardState[2][1][1] == -1 && boardState[2][2][1] == -1 && boardState[2][3][1] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// fourth layer second column
	else if(boardState[3][0][1] == -1 && boardState[3][1][1] == -1 && boardState[3][2][1] == -1 && boardState[3][3][1] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}


////////////////////////////////////////////////////////////Third column of each layer

	// first layer third column
	else if(boardState[0][0][2] == -1 && boardState[0][1][2] == -1 && boardState[0][2][2] == -1 && boardState[0][3][2] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// second layer third column
	else if(boardState[1][0][2] == -1 && boardState[1][1][2] == -1 && boardState[1][2][2] == -1 && boardState[1][3][2] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// third layer third column
	else if(boardState[2][0][2] == -1 && boardState[2][1][2] == -1 && boardState[2][2][2] == -1 && boardState[2][3][2] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// fourth layer third column
	else if(boardState[3][0][2] == -1 && boardState[3][1][2] == -1 && boardState[3][2][2] == -1 && boardState[3][3][2] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}


////////////////////////////////////////////////////////////Fourth column of each layer

	// first layer fourth column
	else if(boardState[0][0][3] == -1 && boardState[0][1][3] == -1 && boardState[0][2][3] == -1 && boardState[0][3][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// second layer fourth column
	else if(boardState[1][0][3] == -1 && boardState[1][1][3] == -1 && boardState[1][2][3] == -1 && boardState[1][3][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// third layer fourth column
	else if(boardState[2][0][3] == -1 && boardState[2][1][3] == -1 && boardState[2][2][3] == -1 && boardState[2][3][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// fourth layer fourth column
	else if(boardState[3][0][3] == -1 && boardState[3][1][3] == -1 && boardState[3][2][3] == -1 && boardState[3][3][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}


////////////////////////////////////////////////////////////First row of each layer

	// first layer first row
	else if(boardState[0][0][0] == -1 && boardState[0][0][1] == -1 && boardState[0][0][2] == -1 && boardState[0][0][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// second layer first row
	else if(boardState[1][0][0] == -1 && boardState[1][0][1] == -1 && boardState[1][0][2] == -1 && boardState[1][0][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// third layer first row
	else if(boardState[2][0][0] == -1 && boardState[2][0][1] == -1 && boardState[2][0][2] == -1 && boardState[2][0][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// fourth layer first row
	else if(boardState[3][0][0] == -1 && boardState[3][0][1] == -1 && boardState[3][0][2] == -1 && boardState[3][0][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}


////////////////////////////////////////////////////////////Second row of each layer

	// first layer second row
	else if(boardState[0][1][0] == -1 && boardState[0][1][1] == -1 && boardState[0][1][2] == -1 && boardState[0][1][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// second layer second row
	else if(boardState[1][1][0] == -1 && boardState[1][1][1] == -1 && boardState[1][1][2] == -1 && boardState[1][1][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// third layer second row
	else if(boardState[2][1][0] == -1 && boardState[2][1][1] == -1 && boardState[2][1][2] == -1 && boardState[2][1][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// fourth layer second row
	else if(boardState[3][1][0] == -1 && boardState[3][1][1] == -1 && boardState[3][1][2] == -1 && boardState[3][1][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}


////////////////////////////////////////////////////////////Third row of each layer

	// first layer third row
	else if(boardState[0][2][0] == -1 && boardState[0][2][1] == -1 && boardState[0][2][2] == -1 && boardState[0][2][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// second layer third row
	else if(boardState[1][2][0] == -1 && boardState[1][2][1] == -1 && boardState[1][2][2] == -1 && boardState[1][2][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// third layer third row
	else if(boardState[2][2][0] == -1 && boardState[2][2][1] == -1 && boardState[2][2][2] == -1 && boardState[2][2][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// fourth layer third row
	else if(boardState[3][2][0] == -1 && boardState[3][1][1] == -1 && boardState[3][2][2] == -1 && boardState[3][2][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}


////////////////////////////////////////////////////////////Fourth row of each layer

	// first layer fourth row
	else if(boardState[0][3][0] == -1 && boardState[0][3][1] == -1 && boardState[0][3][2] == -1 && boardState[0][3][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// second layer fourth row
	else if(boardState[1][3][0] == -1 && boardState[1][3][1] == -1 && boardState[1][3][2] == -1 && boardState[1][3][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// third layer fourth row
	else if(boardState[2][3][0] == -1 && boardState[2][3][1] == -1 && boardState[2][3][2] == -1 && boardState[2][3][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// fourth layer fourth row
	else if(boardState[3][3][0] == -1 && boardState[3][3][1] == -1 && boardState[3][3][2] == -1 && boardState[3][3][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}


////////////////////////////////////////////////////////////Top left diagonal for each layer

	// first layer top left diagonal
	else if(boardState[0][0][0] == -1 && boardState[0][1][1] == -1 && boardState[0][2][2] == -1 && boardState[0][3][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// second layer top left diagonal
	else if(boardState[1][0][0] == -1 && boardState[1][1][1] == -1 && boardState[1][2][2] == -1 && boardState[1][3][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// third layer top left diagonal
	else if(boardState[2][0][0] == -1 && boardState[2][1][1] == -1 && boardState[2][2][2] == -1 && boardState[2][3][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// fourth layer top left diagonal
	else if(boardState[3][0][0] == -1 && boardState[3][1][1] == -1 && boardState[3][2][2] == -1 && boardState[3][3][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}


////////////////////////////////////////////////////////////Top right diagonal for each layer

	// first layer top right diagonal
	else if(boardState[0][0][3] == -1 && boardState[0][1][2] == -1 && boardState[0][2][1] == -1 && boardState[0][3][0] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// second layer top right diagonal
	else if(boardState[1][0][3] == -1 && boardState[1][1][2] == -1 && boardState[1][2][1] == -1 && boardState[1][3][0] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// third layer top right diagonal
	else if(boardState[2][0][3] == -1 && boardState[2][1][2] == -1 && boardState[2][2][1] == -1 && boardState[2][3][0] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// fourth layer top right diagonal
	else if(boardState[3][0][3] == -1 && boardState[3][1][2] == -1 && boardState[3][2][1] == -1 && boardState[3][3][0] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}


////////////////////////////////////////////////////////////Straight up and down for each layer

	// vertical from 0,0 (row, column)
	else if(boardState[0][0][0] == -1 && boardState[1][0][0] == -1 && boardState[2][0][0] == -1 && boardState[3][0][0] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// vertical from 1,0 (row, column)
	else if(boardState[0][1][0] == -1 && boardState[1][1][0] == -1 && boardState[2][1][0] == -1 && boardState[3][1][0] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// vertical from 2,0 (row, column)
	else if(boardState[0][2][0] == -1 && boardState[1][2][0] == -1 && boardState[2][2][0] == -1 && boardState[3][2][0] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// vertical from 3,0 (row, column)
	else if(boardState[0][3][0] == -1 && boardState[1][3][0] == -1 && boardState[2][3][0] == -1 && boardState[3][3][0] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// vertical from 0,1 (row, column)
	else if(boardState[0][0][1] == -1 && boardState[1][0][1] == -1 && boardState[2][0][1] == -1 && boardState[3][0][1] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// vertical from 1,1 (row, column)
	else if(boardState[0][1][1] == -1 && boardState[1][1][1] == -1 && boardState[2][1][1] == -1 && boardState[3][1][1] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// vertical from 2,1 (row, column)
	else if(boardState[0][2][1] == -1 && boardState[1][2][1] == -1 && boardState[2][2][1] == -1 && boardState[3][2][1] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// vertical from 3,1 (row, column)
	else if(boardState[0][3][1] == -1 && boardState[1][3][1] == -1 && boardState[2][3][1] == -1 && boardState[3][3][1] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}
	
	// vertical from 0,2 (row, column)
	else if(boardState[0][0][2] == -1 && boardState[1][0][2] == -1 && boardState[2][0][2] == -1 && boardState[3][0][2] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// vertical from 1,2 (row, column)
	else if(boardState[0][1][2] == -1 && boardState[1][1][2] == -1 && boardState[2][1][2] == -1 && boardState[3][1][2] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// vertical from 2,2 (row, column)
	else if(boardState[0][2][2] == -1 && boardState[1][2][2] == -1 && boardState[2][2][2] == -1 && boardState[3][2][2] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// vertical from 3,2 (row, column)
	else if(boardState[0][3][2] == -1 && boardState[1][3][2] == -1 && boardState[2][3][2] == -1 && boardState[3][3][2] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// vertical from 0,3 (row, column)
	else if(boardState[0][0][3] == -1 && boardState[1][0][3] == -1 && boardState[2][0][3] == -1 && boardState[3][0][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// vertical from 1,3 (row, column)
	else if(boardState[0][1][3] == -1 && boardState[1][1][3] == -1 && boardState[2][1][3] == -1 && boardState[3][1][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// vertical from 2,3 (row, column)
	else if(boardState[0][2][3] == -1 && boardState[1][2][3] == -1 && boardState[2][2][3] == -1 && boardState[3][2][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// vertical from 3,3 (row, column)
	else if(boardState[0][3][3] == -1 && boardState[1][3][3] == -1 && boardState[2][3][3] == -1 && boardState[3][3][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}


////////////////////////////////////////////////////////////Vertical column win but on different layers

	// diagonal up/down from first row first layer
	else if(boardState[0][0][0] == -1 && boardState[1][1][0] == -1 && boardState[2][2][0] == -1 && boardState[3][3][0] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// diagonal up/down from second row second layer
	else if(boardState[0][0][1] == -1 && boardState[1][1][1] == -1 && boardState[2][2][1] == -1 && boardState[3][3][1] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// diagonal up/down from third row third layer
	else if(boardState[0][0][2] == -1 && boardState[1][1][2] == -1 && boardState[2][2][2] == -1 && boardState[3][3][2] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// diagonal up/down from fourth row fourth layer
	else if(boardState[0][0][3] == -1 && boardState[1][1][3] == -1 && boardState[2][2][3] == -1 && boardState[3][3][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// diagonal up/down from fourth row first layer
	else if(boardState[0][3][0] == -1 && boardState[1][2][0] == -1 && boardState[2][1][0] == -1 && boardState[3][0][0] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// diagonal up/down from third row second layer
	else if(boardState[0][3][1] == -1 && boardState[1][2][1] == -1 && boardState[2][1][1] == -1 && boardState[3][0][1] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// diagonal up/down from second row third layer
	else if(boardState[0][3][2] == -1 && boardState[1][2][2] == -1 && boardState[2][1][2] == -1 && boardState[3][0][2] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// diagonal up/down from first row fourth layer
	else if(boardState[0][3][3] == -1 && boardState[1][2][3] == -1 && boardState[2][1][3] == -1 && boardState[3][0][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}


////////////////////////////////////////////////////////////Diagonal win that goes through all layers

	// diagonal vertical from first column
	else if(boardState[0][0][0] == -1 && boardState[1][1][1] == -1 && boardState[2][2][2] == -1 && boardState[3][3][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// diagonal vertical from fourth column
	else if(boardState[0][0][3] == -1 && boardState[1][1][2] == -1 && boardState[2][2][1] == -1 && boardState[3][3][0] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// diagonal vertical from fourth column first layer
	else if(boardState[0][3][3] == -1 && boardState[1][2][2] == -1 && boardState[2][1][1] == -1 && boardState[3][0][0] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// diagonal vertical from first column first layer
	else if(boardState[0][3][0] == -1 && boardState[1][2][1] == -1 && boardState[2][1][2] == -1 && boardState[3][0][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}


////////////////////////////////////////////////////////////Horizontal win across all layers starting from the left

	// first row horizontal across all layers from first column
	else if(boardState[0][0][0] == -1 && boardState[1][0][1] == -1 && boardState[2][0][2] == -1 && boardState[3][0][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// second row horizontal across all layers from first column
	else if(boardState[0][1][0] == -1 && boardState[1][1][1] == -1 && boardState[2][1][2] == -1 && boardState[3][1][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// third row horizontal across all layers from first column
	else if(boardState[0][2][0] == -1 && boardState[1][2][1] == -1 && boardState[2][2][2] == -1 && boardState[3][2][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// fourth row horizontal across all layers from first column
	else if(boardState[0][3][0] == -1 && boardState[1][3][1] == -1 && boardState[2][3][2] == -1 && boardState[3][3][3] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}


////////////////////////////////////////////////////////////Horizontal win across all layers starting from the right

	// first row horizontal across all layers from last column
	else if(boardState[0][0][3] == -1 && boardState[1][0][2] == -1 && boardState[2][0][1] == -1 && boardState[3][0][0] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// second row horizontal across all layers from last column
	else if(boardState[0][1][3] == -1 && boardState[1][1][2] == -1 && boardState[2][1][1] == -1 && boardState[3][1][0] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// third row horizontal across all layers from last column
	else if(boardState[0][2][3] == -1 && boardState[1][2][2] == -1 && boardState[2][2][1] == -1 && boardState[3][2][0] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}

	// fourth row horizontal across all layers from last column
	else if(boardState[0][3][3] == -1 && boardState[1][3][2] == -1 && boardState[2][3][1] == -1 && boardState[3][3][0] == -1)
	{
		if(isAIx == true)
			return 999;

		else if(isAIo == true)
			return -999;
	}



/*----------------------------------------------------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////Wins for o's


////////////////////////////////////////////////////////////First column of each layer

	// first layer first column
	else if(boardState[0][0][0] == 1 && boardState[0][1][0] == 1 && boardState[0][2][0] == 1 && boardState[0][3][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// second layer first column
	else if(boardState[1][0][0] == 1 && boardState[1][1][0] == 1 && boardState[1][2][0] == 1 && boardState[1][3][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// third layer first column
	else if(boardState[2][0][0] == 1 && boardState[2][1][0] == 1 && boardState[2][2][0] == 1 && boardState[2][3][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// fourth layer first column
	else if(boardState[3][0][0] == 1 && boardState[3][1][0] == 1 && boardState[3][2][0] == 1 && boardState[3][3][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}


////////////////////////////////////////////////////////////Second column of each layer

	// first layer second column
	else if(boardState[0][0][1] == 1 && boardState[0][1][1] == 1 && boardState[0][2][1] == 1 && boardState[0][3][1] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// second layer second column
	else if(boardState[1][0][1] == 1 && boardState[1][1][1] == 1 && boardState[1][2][1] == 1 && boardState[1][3][1] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// third layer second column
	else if(boardState[2][0][1] == 1 && boardState[2][1][1] == 1 && boardState[2][2][1] == 1 && boardState[2][3][1] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// fourth layer second column
	else if(boardState[3][0][1] == 1 && boardState[3][1][1] == 1 && boardState[3][2][1] == 1 && boardState[3][3][1] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}


////////////////////////////////////////////////////////////Third column of each layer

	// first layer third column
	else if(boardState[0][0][2] == 1 && boardState[0][1][2] == 1 && boardState[0][2][2] == 1 && boardState[0][3][2] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// second layer third column
	else if(boardState[1][0][2] == 1 && boardState[1][1][2] == 1 && boardState[1][2][2] == 1 && boardState[1][3][2] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// third layer third column
	else if(boardState[2][0][2] == 1 && boardState[2][1][2] == 1 && boardState[2][2][2] == 1 && boardState[2][3][2] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// fourth layer third column
	else if(boardState[3][0][2] == 1 && boardState[3][1][2] == 1 && boardState[3][2][2] == 1 && boardState[3][3][2] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}


////////////////////////////////////////////////////////////Fourth column of each layer

	// first layer fourth column
	else if(boardState[0][0][3] == 1 && boardState[0][1][3] == 1 && boardState[0][2][3] == 1 && boardState[0][3][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// second layer fourth column
	else if(boardState[1][0][3] == 1 && boardState[1][1][3] == 1 && boardState[1][2][3] == 1 && boardState[1][3][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// third layer fourth column
	else if(boardState[2][0][3] == 1 && boardState[2][1][3] == 1 && boardState[2][2][3] == 1 && boardState[2][3][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// fourth layer fourth column
	else if(boardState[3][0][3] == 1 && boardState[3][1][3] == 1 && boardState[3][2][3] == 1 && boardState[3][3][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}


////////////////////////////////////////////////////////////First row of each layer

	// first layer first row
	else if(boardState[0][0][0] == 1 && boardState[0][0][1] == 1 && boardState[0][0][2] == 1 && boardState[0][0][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// second layer first row
	else if(boardState[1][0][0] == 1 && boardState[1][0][1] == 1 && boardState[1][0][2] == 1 && boardState[1][0][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// third layer first row
	else if(boardState[2][0][0] == 1 && boardState[2][0][1] == 1 && boardState[2][0][2] == 1 && boardState[2][0][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// fourth layer first row
	else if(boardState[3][0][0] == 1 && boardState[3][0][1] == 1 && boardState[3][0][2] == 1 && boardState[3][0][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}


////////////////////////////////////////////////////////////Second row of each layer

	// first layer second row
	else if(boardState[0][1][0] == 1 && boardState[0][1][1] == 1 && boardState[0][1][2] == 1 && boardState[0][1][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// second layer second row
	else if(boardState[1][1][0] == 1 && boardState[1][1][1] == 1 && boardState[1][1][2] == 1 && boardState[1][1][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// third layer second row
	else if(boardState[2][1][0] == 1 && boardState[2][1][1] == 1 && boardState[2][1][2] == 1 && boardState[2][1][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// fourth layer second row
	else if(boardState[3][1][0] == 1 && boardState[3][1][1] == 1 && boardState[3][1][2] == 1 && boardState[3][1][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}


////////////////////////////////////////////////////////////Third row of each layer

	// first layer third row
	else if(boardState[0][2][0] == 1 && boardState[0][2][1] == 1 && boardState[0][2][2] == 1 && boardState[0][2][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// second layer third row
	else if(boardState[1][2][0] == 1 && boardState[1][2][1] == 1 && boardState[1][2][2] == 1 && boardState[1][2][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// third layer third row
	else if(boardState[2][2][0] == 1 && boardState[2][2][1] == 1 && boardState[2][2][2] == 1 && boardState[2][2][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// fourth layer third row
	else if(boardState[3][2][0] == 1 && boardState[3][1][1] == 1 && boardState[3][2][2] == 1 && boardState[3][2][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}


////////////////////////////////////////////////////////////Fourth row of each layer

	// first layer fourth row
	else if(boardState[0][3][0] == 1 && boardState[0][3][1] == 1 && boardState[0][3][2] == 1 && boardState[0][3][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// second layer fourth row
	else if(boardState[1][3][0] == 1 && boardState[1][3][1] == 1 && boardState[1][3][2] == 1 && boardState[1][3][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// third layer fourth row
	else if(boardState[2][3][0] == 1 && boardState[2][3][1] == 1 && boardState[2][3][2] == 1 && boardState[2][3][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// fourth layer fourth row
	else if(boardState[3][3][0] == 1 && boardState[3][3][1] == 1 && boardState[3][3][2] == 1 && boardState[3][3][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	
////////////////////////////////////////////////////////////Top left diagonal for each layer

	// first layer top left diagonal
	else if(boardState[0][0][0] == 1 && boardState[0][1][1] == 1 && boardState[0][2][2] == 1 && boardState[0][3][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// second layer top left diagonal
	else if(boardState[1][0][0] == 1 && boardState[1][1][1] == 1 && boardState[1][2][2] == 1 && boardState[1][3][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// third layer top left diagonal
	else if(boardState[2][0][0] == 1 && boardState[2][1][1] == 1 && boardState[2][2][2] == 1 && boardState[2][3][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// fourth layer top left diagonal
	else if(boardState[3][0][0] == 1 && boardState[3][1][1] == 1 && boardState[3][2][2] == 1 && boardState[3][3][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}


////////////////////////////////////////////////////////////Top right diagonal for each layer

	// first layer top right diagonal
	else if(boardState[0][0][3] == 1 && boardState[0][1][2] == 1 && boardState[0][2][1] == 1 && boardState[0][3][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// second layer top right diagonal
	else if(boardState[1][0][3] == 1 && boardState[1][1][2] == 1 && boardState[1][2][1] == 1 && boardState[1][3][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// third layer top right diagonal
	else if(boardState[2][0][3] == 1 && boardState[2][1][2] == 1 && boardState[2][2][1] == 1 && boardState[2][3][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// fourth layer top right diagonal
	else if(boardState[3][0][3] == 1 && boardState[3][1][2] == 1 && boardState[3][2][1] == 1 && boardState[3][3][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}


////////////////////////////////////////////////////////////Straight up and down for each layer

	// vertical from 0,0 (row, column)
	else if(boardState[0][0][0] == 1 && boardState[1][0][0] == 1 && boardState[2][0][0] == 1 && boardState[3][0][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// vertical from 1,0 (row, column)
	else if(boardState[0][1][0] == 1 && boardState[1][1][0] == 1 && boardState[2][1][0] == 1 && boardState[3][1][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// vertical from 2,0 (row, column)
	else if(boardState[0][2][0] == 1 && boardState[1][2][0] == 1 && boardState[2][2][0] == 1 && boardState[3][2][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// vertical from 3,0 (row, column)
	else if(boardState[0][3][0] == 1 && boardState[1][3][0] == 1 && boardState[2][3][0] == 1 && boardState[3][3][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// vertical from 0,1 (row, column)
	else if(boardState[0][0][1] == 1 && boardState[1][0][1] == 1 && boardState[2][0][1] == 1 && boardState[3][0][1] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// vertical from 1,1 (row, column)
	else if(boardState[0][1][1] == 1 && boardState[1][1][1] == 1 && boardState[2][1][1] == 1 && boardState[3][1][1] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// vertical from 2,1 (row, column)
	else if(boardState[0][2][1] == 1 && boardState[1][2][1] == 1 && boardState[2][2][1] == 1 && boardState[3][2][1] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// vertical from 3,1 (row, column)
	else if(boardState[0][3][1] == 1 && boardState[1][3][1] == 1 && boardState[2][3][1] == 1 && boardState[3][3][1] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}
	
	// vertical from 0,2 (row, column)
	else if(boardState[0][0][2] == 1 && boardState[1][0][2] == 1 && boardState[2][0][2] == 1 && boardState[3][0][2] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// vertical from 1,2 (row, column)
	else if(boardState[0][1][2] == 1 && boardState[1][1][2] == 1 && boardState[2][1][2] == 1 && boardState[3][1][2] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// vertical from 2,2 (row, column)
	else if(boardState[0][2][2] == 1 && boardState[1][2][2] == 1 && boardState[2][2][2] == 1 && boardState[3][2][2] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// vertical from 3,2 (row, column)
	else if(boardState[0][3][2] == 1 && boardState[1][3][2] == 1 && boardState[2][3][2] == 1 && boardState[3][3][2] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}


	// vertical from 0,3 (row, column)
	else if(boardState[0][0][3] == 1 && boardState[1][0][3] == 1 && boardState[2][0][3] == 1 && boardState[3][0][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// vertical from 1,3 (row, column)
	else if(boardState[0][1][3] == 1 && boardState[1][1][3] == 1 && boardState[2][1][3] == 1 && boardState[3][1][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// vertical from 2,3 (row, column)
	else if(boardState[0][2][3] == 1 && boardState[1][2][3] == 1 && boardState[2][2][3] == 1 && boardState[3][2][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// vertical from 3,3 (row, column)
	else if(boardState[0][3][3] == 1 && boardState[1][3][3] == 1 && boardState[2][3][3] == 1 && boardState[3][3][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}


////////////////////////////////////////////////////////////Vertical column win but on different layers

	// diagonal up/down from first row first layer
	else if(boardState[0][0][0] == 1 && boardState[1][1][0] == 1 && boardState[2][2][0] == 1 && boardState[3][3][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// diagonal up/down from second column second layer
	else if(boardState[0][0][1] == 1 && boardState[1][1][1] == 1 && boardState[2][2][1] == 1 && boardState[3][3][1] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// diagonal up/down from third column third layer
	else if(boardState[0][0][2] == 1 && boardState[1][1][2] == 1 && boardState[2][2][2] == 1 && boardState[3][3][2] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// diagonal up/down from fourth column fourth layer
	else if(boardState[0][0][3] == 1 && boardState[1][1][3] == 1 && boardState[2][2][3] == 1 && boardState[3][3][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// diagonal up/down from fourth row first layer
	else if(boardState[0][3][0] == 1 && boardState[1][2][0] == 1 && boardState[2][1][0] == 1 && boardState[3][0][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// diagonal up/down from third row second layer
	else if(boardState[0][3][1] == 1 && boardState[1][2][1] == 1 && boardState[2][1][1] == 1 && boardState[3][0][1] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// diagonal up/down from second row third layer
	else if(boardState[0][3][2] == 1 && boardState[1][2][2] == 1 && boardState[2][1][2] == 1 && boardState[3][0][2] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// diagonal up/down from first row fourth layer
	else if(boardState[0][3][3] == 1 && boardState[1][2][3] == 1 && boardState[2][1][3] == 1 && boardState[3][0][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}


////////////////////////////////////////////////////////////Diagonal win that goes through all layers

	// diagonal vertical from first column
	else if(boardState[0][0][0] == 1 && boardState[1][1][1] == 1 && boardState[2][2][2] == 1 && boardState[3][3][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// diagonal vertical from fourth column
	else if(boardState[0][0][3] == 1 && boardState[1][1][2] == 1 && boardState[2][2][1] == 1 && boardState[3][3][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}




	// diagonal vertical from fourth column first layer
	else if(boardState[0][3][3] == 1 && boardState[1][2][2] == 1 && boardState[2][1][1] == 1 && boardState[3][0][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// diagonal vertical from first column first layer
	else if(boardState[0][3][0] == 1 && boardState[1][2][1] == 1 && boardState[2][1][2] == 1 && boardState[3][0][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}


////////////////////////////////////////////////////////////Horizontal win across all layers starting from the left

	// first row horizontal across all layers from first column
	else if(boardState[0][0][0] == 1 && boardState[1][0][1] == 1 && boardState[2][0][2] == 1 && boardState[3][0][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// second row horizontal across all layers from first column
	else if(boardState[0][1][0] == 1 && boardState[1][1][1] == 1 && boardState[2][1][2] == 1 && boardState[3][1][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// third row horizontal across all layers from first column
	else if(boardState[0][2][0] == 1 && boardState[1][2][1] == 1 && boardState[2][2][2] == 1 && boardState[3][2][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// fourth row horizontal across all layers from first column
	else if(boardState[0][3][0] == 1 && boardState[1][3][1] == 1 && boardState[2][3][2] == 1 && boardState[3][3][3] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}


////////////////////////////////////////////////////////////Horizontal win across all layers starting from the right

	// first row horizontal across all layers from last column
	else if(boardState[0][0][3] == 1 && boardState[1][0][2] == 1 && boardState[2][0][1] == 1 && boardState[3][0][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// second row horizontal across all layers from last column
	else if(boardState[0][1][3] == 1 && boardState[1][1][2] == 1 && boardState[2][1][1] == 1 && boardState[3][1][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// third row horizontal across all layers from last column
	else if(boardState[0][2][3] == 1 && boardState[1][2][2] == 1 && boardState[2][2][1] == 1 && boardState[3][2][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}

	// fourth row horizontal across all layers from last column
	else if(boardState[0][3][3] == 1 && boardState[1][3][2] == 1 && boardState[2][3][1] == 1 && boardState[3][3][0] == 1)
	{
		if(isAIx == true)
			return -999;

		else if(isAIo == true)
			return 999;
	}



}

/*
	whoIsAI: decides if AI is x or o
	args: none
	returns: void
	Reads the state of the board and counts the number of x's and o's.
	If there are more o's than x's then AI is x. Same number of x's and o's 
	means AI is o.
*/
void TTTBoard::whoIsAI()
{
	// local variables
	int numX = 0;
	int numO = 0;

	// loop through entire board
	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			for( int k = 0; k < 4; k++ )
			{
				// count number of x's
				if(boardState[i][j][k] == -1)
					numX++;

				// count number of o's
				else if(boardState[i][j][k] == 1)
					numO++;
			}
		}
	}

	// AI is o
	if(numO == numX)
	{
		isAIo = true;
		isAIx = false;
		AITurn = true;
	}

	// AI is x
	else if(numO > numX)
	{
		isAIo = false;
		isAIx = true;
		AITurn = true;
	}

	// invalid board since o goes first
	else
		cout << "More x's than o's." << endl;
}

/*
	calcMinMax: calculates minmax value
	args: int currentPly, int maxPly
	returns: void
	Calculates the minmax value of various states of the board. 
	Also makes use of Alphe-Beta pruning to prune off unncessary 
	parts of the sreach tree.
*/
void TTTBoard::calcMinMax(int currentPly, int maxPly)
{
	// local variables
	int i, j, k;

	// recurse as long as max play has not been met
	if(currentPly < maxPly)
	{
		// loop through entire board
		for( i = 0; i < 4; i++ )
		{
			for( j = 0; j < 4; j++ )
			{
				for( k = 0; k < 4; k++ )
				{
					// prune off segments of the tree
					if(boardState[i][j][k] == 0 && score != 999 && score != -999)
					{
						// create new board state
						children[i][j][k] = new TTTBoard;
						children[i][j][k]->copyBoard(boardState);
						children[i][j][k]->parent = this;

						// insert x or o into new board state
						if(isAIx == true && AITurn == true)
							children[i][j][k]->boardState[i][j][k] = -1;

						else if(isAIo == true && AITurn == true)
							children[i][j][k]->boardState[i][j][k] = 1;

						else if(isAIx == true && AITurn == false)
							children[i][j][k]->boardState[i][j][k] = 1;

						else if(isAIo == true && AITurn == false)
							children[i][j][k]->boardState[i][j][k] = -1;

						// decide if it is the AI's turn
						if(AITurn == true)
							children[i][j][k]->AITurn = false;

						else if(AITurn == false)
							children[i][j][k]->AITurn = true;

						// set whether AI is x or o
						if(isAIx == true)
						{
							children[i][j][k]->isAIx = true;
							children[i][j][k]->isAIo = false;		
						}

						else if(isAIo == true)
						{
							children[i][j][k]->isAIx = false;
							children[i][j][k]->isAIo = true;		
						}

						// save values of where the new board state value has been placed
						children[i][j][k]->layer = i;
						children[i][j][k]->row = j;
						children[i][j][k]->column = k;

						// set values for pruning
						if(children[i][j][k]->winDetector() == 999 && AITurn == true)
						{
							score = 999;
							children[i][j][k]->score = 999;
						}

						// set values for pruning
						else if(children[i][j][k]->winDetector() == -999 && AITurn == false)
						{
							score = -999;
							children[i][j][k]->score = -999;
						}

						// recurse with new board state
						else 
							children[i][j][k]->calcMinMax(currentPly + 1, maxPly);
					}
				}
			}
		}
	}

	// evulaute value of terminal node
	else if(currentPly == maxPly)
	{
		// set how good the state of the voard is
		scoreEvaluator();

		// check if board state ends in a win
		if(winDetector() == 999)
			score = 999;

		// check if board state ends in a loss
		else if(winDetector() == -999)
			score = -999;
	}

	// check if we are not doing the first ply
	if(currentPly != 0)
	{
		// lopp through entire board
		for( i = 0; i < 4; i++ )
		{
			for( j = 0; j < 4; j++ )
			{
				for( k = 0; k < 4; k++ )
				{
					// set first score value
					if(children[i][j][k] != NULL && firstValueSet == false)
					{
						firstValueSet = true;
						score = children[i][j][k]->score;
					}
				
					// set larger score value if it is our turn
					else if(children[i][j][k] != NULL && AITurn == true && score < children[i][j][k]->score)
						score = children[i][j][k]->score;

					// set smaller score value if it is the opponents turn
					else if(children[i][j][k] != NULL && AITurn == false && score > children[i][j][k]->score)
						score = children[i][j][k]->score;
				}
			}
		}
	}

	// check if we are doing the first ply
	if(currentPly == 0)
	{
		// loop through entire board
		for( i = 0; i < 4; i++ )
		{
			for( j = 0; j < 4; j++ )
			{
				for( k = 0; k < 4; k++ )
				{
					// set first score value
					if(children[i][j][k] != NULL && firstValueSet == false)
					{
						// set first to true
						firstValueSet = true;
						
						// get score value
						score = children[i][j][k]->score;

						// get coordinates of where x or o was placed
						layer = children[i][j][k]->layer;
						row = children[i][j][k]->row;
						column = children[i][j][k]->column;
					}
		
					// get largest score value since we know its our turn
					else if(children[i][j][k] != NULL && score < children[i][j][k]->score)
					{
						// get score value
						score = children[i][j][k]->score;

						// get coordinates of where x or o was placed
						layer = children[i][j][k]->layer;
						row = children[i][j][k]->row;
						column = children[i][j][k]->column;
					}
				}
			}
		}
	}
}

/*
	read_from_file: reads the board from a file
	args:
		char* inputfilename: filename to be read
		int board[][4][4]: array to store board
	returns: 0 if all is well; -2 if the file cannot be opened for reading
*/

int read_from_file( char* inputfilename, int board[][4][4] );

/*
	write_to_file: writes the board from a file
	args:
		char* outputfilename: filename to be written
		int board[4][4][4]: array to store board
	returns: 0 if all is well; -2 if the file cannot be opened for writing
*/

int write_to_file( char* outputfilename, int board[4][4][4] );

/*
	print_move_to_screen: prints the board so that the autograder should be able to read your answer
	args:
		Move m: struct storing the x,y,z coordinate of the square to move to
	returns: void
*/
void print_move_to_screen( Move m );


int main( int argc, char* argv[] )
{
	// local variables
	char* inputfilename = NULL, *outputfilename = NULL;
	int board[4][4][4];
	Move m = {0,0,0};
	int ply = 0;
	TTTBoard TTTBoard;

	// parse command-line arguments
	for( int i = 1; i < argc; i++ )
	{
		// input file
		if( !strcmp(argv[i], "-i" ) )
		{
			inputfilename = argv[i+1];
			i++;
		}
		// output file
		else if( !strcmp( argv[i], "-o") )
		{
			outputfilename = argv[i+1];
			i++;
		}
		// number of ply to search ahead
		else if( !strcmp( argv[i], "-p") )
		{
			ply = atoi(argv[i+1]);
			i++;
		}
	}

	// check to make sure command-line arguments have been specified
	if( inputfilename == NULL || outputfilename == NULL )
	{
		printf( "input and output filenames need to be specified on the command line (-i <filename> -o <filename>\n");
		return -1;
	}

	if( ply <= 0 )
	{
		printf( "need to have ply set to be greater than 0 (use -p <ply>)\n");
		return -1;
	}

	// debug info
	//printf( "input file: [%s]\n", inputfilename);
	//printf( "output file: [%s]\n", outputfilename);

	// read from file
	read_from_file( inputfilename, board );

	// debug
	//printf( "starting tictactoe\n");

	//sleep(5);

	//debug into
	//printf( "finishing tictactoe\n");
	
	// get read in state of board
	TTTBoard.copyBoard(board);

	// determine if AI is x or o
	TTTBoard.whoIsAI();

	// caluclate minmax value with the given ply
	TTTBoard.calcMinMax(0, ply);

	if(TTTBoard.isAIx == true)
		board[TTTBoard.layer][TTTBoard.row][TTTBoard.column] = -1;

	else if(TTTBoard.isAIo == true)
		board[TTTBoard.layer][TTTBoard.row][TTTBoard.column] = 1;
	
	// copy move to move struct
	m.x = TTTBoard.layer;
	m.y = TTTBoard.row;
	m.z = TTTBoard.column;

	// print AI move ot screen
	print_move_to_screen( m );

	// debugging
	//cout << "this is the score: " << TTTBoard.score << endl;

	//write board state to file and exit
	return write_to_file(outputfilename, board);
}

int read_from_file( char* inputfilename, int board[][4][4] )
{
	FILE *ifile = fopen( inputfilename, "r" );
	if( !ifile )
	{
		printf( "could not open input file [%s] for reading\n", inputfilename );
		return -2;
	}

	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			for( int k = 0; k < 4; k++ )
			{
				char c = '.';
				fscanf(ifile, " %c", &c );
				switch( c )
				{
					case '.': board[i][j][k] = 0; break;
					case 'x': board[i][j][k] = -1; break;
					case 'o': board[i][j][k] = 1; break;
					default: board[i][j][k] = 0; break;
				}

			}
		}
	}

	fclose( ifile );
	return 0;
}

int write_to_file( char* outputfilename, int board[4][4][4] )
{
	FILE *ofile = fopen( outputfilename, "w" );
	if( !ofile )
	{
		printf( "could not open output file [%s] for writing\n", outputfilename );
		return -2;
	}

	// iterate through all squares on the board
	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			for( int k = 0; k < 4; k++ )
			{
				// write the appropriate character to the file
				switch( board[i][j][k])
				{
					case 0: fprintf( ofile, "%c", '.'); break;
					case 1: fprintf( ofile, "%c", 'o'); break;
					case -1: fprintf( ofile, "%c", 'x'); break;
					default: fprintf( ofile, "%c", '.'); break;
				}
			}
			fprintf( ofile, "\n");
		}
		fprintf( ofile, "\n");
	}

	// close the output file
	fclose (ofile);
	return 0;
}

void print_move_to_screen( Move m)
{
	printf( "%d %d %d\n", m.x, m.y, m.z);
}




