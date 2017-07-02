#ifndef __Board_H__
#define __Board_H__

#define ROWS 6
#define COLS 7

int depthLimit = 5;	

enum Players 
{
    NONE,
    AI,
    PLAYER
};

class Board 
{
    int board[ROWS][COLS];
    bool horizontalStreak(int row, int player);
    bool verticalStreak(int col, int player);
    bool topRightDiagonal(int player);
    bool topLeftDiagonal(int player);
public:
    Board();
    int getNode(int row, int col);
    void printBoard();
    bool makeMove(int col, int player);
    bool isATie();
    int isAWin();
};

#endif // Board_H
