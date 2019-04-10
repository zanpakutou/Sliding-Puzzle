#ifndef _BOARD_H
#define _BOARD_H

struct board
{
    int pivot, e[3][3];
    board();
    board(int n);
    board moveUp();
    board moveDown();
    board moveLeft();
    board moveRigh();
    int getID();
};
#endif // _BOARD_H
