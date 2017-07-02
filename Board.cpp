#include <iostream>
#include <cstdlib>
#include "Board.h"

Board::Board()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			board[i][j] = NONE;
		}
	}
}

int Board::getNode(int row, int col)
{
	return board[row][col];
}

bool Board::makeMove(int col, int player)
{
	for (int i = ROWS - 1; i >= 0; i--)
	{
		if (board[i][col] == 0)
		{
			board[i][col] = player;
			return true;
		}
	}

	return false;
}

bool Board::horizontalStreak(int row, int player)
{
	for (int i = 0; i < COLS - 3; i++)
	{
		if (board[row][i] == player &&
			board[row][i + 1] == player &&
			board[row][i + 2] == player &&
			board[row][i + 3] == player)
			return true;

	}
	return false;
}

bool Board::verticalStreak(int col, int player)
{
	for (int i = 0; i < ROWS - 3; i++)
	{
		if (board[i][col] == player &&
			board[i + 1][col] == player &&
			board[i + 2][col] == player &&
			board[i + 3][col] == player)
			return true;
	}
	return false;
}

bool Board::topRightDiagonal(int player)
{
	for (int i = ROWS - 1; i > 2; i--)
	{
		for (int j = 0; j < COLS - 3; j++)
		{
			if (board[i][j] == player &&
				board[i - 1][j + 1] == player &&
				board[i - 2][j + 2] == player &&
				board[i - 3][j + 3] == player)
				return true;
		}
	}

	return false;
}

bool Board::topLeftDiagonal(int player)
{
	for (int i = 0; i < ROWS - 3; i++)
	{
		for (int j = 0; j < COLS - 3; j++)
		{
			if (board[i][j] == player &&
				board[i + 1][j + 1] == player &&
				board[i + 2][j + 2] == player &&
				board[i + 3][j + 3] == player)
				return true;
		}
	}

	return false;
}

int Board::isAWin()
{
	for (int i = 0; i < ROWS; i++)
	{
		if (horizontalStreak(i, AI))
			return 1;
		else if (horizontalStreak(i, PLAYER))
			return -1;
	}

	for (int i = 0; i < COLS; i++)
	{
		if (verticalStreak(i, AI))
			return 1;
		else if (verticalStreak(i, PLAYER))
			return -1;
	}

	if (topRightDiagonal(AI))
		return 1;
	else if (topRightDiagonal(PLAYER))
		return -1;


	if (topLeftDiagonal(AI))
		return 1;
	else if (topLeftDiagonal(PLAYER))
		return -1;

	return 0;
}

bool Board::isATie()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (board[i][j] == NONE)
				return false;
		}
	}

	return true;
}

int calculateScore(int aiScore, int moreMoves)
{
	int moveScore = 4 - moreMoves;
	if (aiScore == 0) return 0;
	else if (aiScore == 1) return 1 * moveScore;
	else if (aiScore == 2) return 10 * moveScore;
	else if (aiScore == 3) return 1000 * moveScore;
	else return 10e6;
}

int evaluate(Board b)
{

	int aiScore = 1;
	int score = 0;
	int blanks = 0;
	int k = 0, moreMoves = 0;
	for (int i = ROWS - 1; i >= 0; --i)
	{
		for (int j = 0; j <= COLS - 1; ++j)
		{

			if (b.getNode(i, j) == 0 || b.getNode(i, j) == 2) continue;

			if (j <= COLS - 4)
			{
				for (k = 1; k < 4; ++k)
				{
					if (b.getNode(i, j + k) == 1)aiScore++;
					else if (b.getNode(i, j + k) == 2)
					{
						aiScore = 0;
						blanks = 0;
						break;
					}
					else blanks++;
				}

				moreMoves = 0;
				if (blanks > 0)
					for (int c = 1; c < 4; ++c)
					{
						int column = j + c;
						for (int m = i; m <= 5; m++)
						{
							if (b.getNode(m, column) == 0)moreMoves++;
							else break;
						}
					}

				if (moreMoves != 0) score += calculateScore(aiScore, moreMoves);
				aiScore = 1;
				blanks = 0;
			}

			if (i >= 3)
			{
				for (k = 1; k < 4; ++k)
				{
					if (b.getNode(i - k, j) == 1)aiScore++;
					else if (b.getNode(i - k, j) == 2)
					{
						aiScore = 0;
						break;
					}
				}
				moreMoves = 0;

				if (aiScore > 0)
				{
					int column = j;
					for (int m = i - k + 1; m <= i - 1; m++)
					{
						if (b.getNode(m, column) == 0)moreMoves++;
						else break;
					}
				}
				if (moreMoves != 0) score += calculateScore(aiScore, moreMoves);
				aiScore = 1;
				blanks = 0;
			}

			if (j >= 3)
			{
				for (k = 1; k < 4; ++k)
				{
					if (b.getNode(i, j - k) == 1)aiScore++;
					else if (b.getNode(i, j - k) == 2)
					{
						aiScore = 0;
						blanks = 0;
						break;
					}
					else blanks++;
				}
				moreMoves = 0;
				if (blanks > 0)
					for (int c = 1; c < 4; ++c)
					{
						int column = j - c;
						for (int m = i; m <= 5; m++)
						{
							if (b.getNode(m, column) == 0)moreMoves++;
							else break;
						}
					}

				if (moreMoves != 0) score += calculateScore(aiScore, moreMoves);
				aiScore = 1;
				blanks = 0;
			}

			if (j <= COLS - 4 && i >= 3)
			{
				for (k = 1; k < 4; ++k)
				{
					if (b.getNode(i - k, j + k) == 1)aiScore++;
					else if (b.getNode(i - k, j + k) == 2)
					{
						aiScore = 0;
						blanks = 0;
						break;
					}
					else blanks++;
				}
				moreMoves = 0;
				if (blanks > 0)
				{
					for (int c = 1; c < 4; ++c)
					{
						int column = j + c, row = i - c;
						for (int m = row; m <= 5; ++m)
						{
							if (b.getNode(m, column) == 0)moreMoves++;
							else if (b.getNode(m, column) == 1);
							else break;
						}
					}
					if (moreMoves != 0) score += calculateScore(aiScore, moreMoves);
					aiScore = 1;
					blanks = 0;
				}
			}

			if (i >= 3 && j >= 3)
			{
				for (k = 1; k < 4; ++k)
				{
					if (b.getNode(i - k, j - k) == 1)aiScore++;
					else if (b.getNode(i - k, j - k) == 2)
					{
						aiScore = 0;
						blanks = 0;
						break;
					}
					else blanks++;
				}
				moreMoves = 0;
				if (blanks > 0)
				{
					for (int c = 1; c < 4; ++c)
					{
						int column = j - c, row = i - c;
						for (int m = row; m <= 5; ++m)
						{
							if (b.getNode(m, column) == 0)moreMoves++;
							else if (b.getNode(m, column) == 1);
							else break;
						}
					}
					if (moreMoves != 0) score += calculateScore(aiScore, moreMoves);
					aiScore = 1;
					blanks = 0;
				}
			}
		}
	}
	return score;
}

int firstPlayer(Board b, int depth, int alpha, int beta);

int secondPlayer(Board b, int depth, int alpha, int beta)
{
	int bestValue;

	if (b.isAWin() == 1)
		return 10e6;
	else if (b.isATie())
		return 0;
	else if (b.isAWin() == -1)
		return -10e6;
	else if (depth == depthLimit)
		return evaluate(b);

	int bestMove = 0;
	for (int i = 0; i < COLS; i++)
	{
		Board nb = b;
		if (nb.getNode(0, i) == 0)
		{
			nb.makeMove(i, 2);
			bestValue = firstPlayer(nb, depth + 1, alpha, beta);

			if (bestValue < beta)
			{
				beta = bestValue;
				bestMove = i;
			}
		}
	}

	if (depth == 0) return bestMove;

	return beta;
}

int firstPlayer(Board b, int depth, int alpha, int beta)
{
	int bestValue;

	if (b.isAWin() == 1)
		return 10e6;
	else if (b.isATie())
		return 0;
	else if (b.isAWin() == -1)
		return -10e6;
	else if (depth == depthLimit)
		return evaluate(b);

	int bestMove = 0;
	for (int i = 0; i < COLS; i++)
	{
		Board nb = b;
		if (nb.getNode(0, i) == 0)
		{
			nb.makeMove(i, 1);
			bestValue = secondPlayer(nb, depth + 1, alpha, beta);

			if (bestValue > alpha)
			{
				alpha = bestValue;
				bestMove = i;
			}
		}
	}

	if (depth == 0) return bestMove;

	return alpha;
}

void Board::printBoard()
{
	std::cout << "    ";
	for (int i = 0; i < COLS; i++)
		std::cout << i << "  ";
	std::cout << std::endl << std::endl;
	for (int i = 0; i < ROWS; i++)
	{
		std::cout << i << " ";
		for (int j = 0; j < COLS; j++)
		{
			std::cout << " |" << board[i][j];
		}
		std::cout << " |" << std::endl;
	}
	std::cout << std::endl;
}

int main()
{
	int col;
	Board board;
	board.printBoard();

	while (true)
	{
		if (board.isATie())
		{
			std::cout << "Tie!";
			break;
		}

		col = firstPlayer(board, 0, -10e6, 10e6);

		std::cout << "AI move: " << col << std::endl << std::endl;

		board.makeMove(col, AI);
		board.printBoard();

		if (board.isAWin() == 1)
		{
			std::cout << "AI won the game!";
			break;
		}

		std::cout << "Player move: ";
		std::cin >> col;
		std::cout << std::endl;

		while (col < 0 || col >= COLS)
		{
			std::cout << "Illegal move!" << std::endl;
			std::cout << "Player move: ";
			std::cin >> col;
		}

		board.makeMove(col, PLAYER);
		board.printBoard();

		if (board.isAWin() == -1)
		{
			std::cout << "Player won the game!";
			break;
		}
	}
	std::cout << std::endl;

	return 0;
}