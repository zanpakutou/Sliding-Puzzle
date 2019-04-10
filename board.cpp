#include "board.h"
#include <algorithm>

const int N = 3;

board::board() = default;
board::board(int n)
{
    int length = 8;
    while(n){
        e[length/3][length % 3] = n % 10;
        n/= 10;
        length--;
    }
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++) if (e[i][j] == 1) pivot = i * 3 + j;
}
board board::moveUp()
{
    if (pivot < 3) return board(getID());
    board newboard = *this;
    std::swap(newboard.e[pivot/3][pivot%3], newboard.e[pivot/3 - 1][pivot % 3]);
    newboard.pivot-= 3;
    return newboard;
}
board board::moveDown()
{
    if (pivot > 5) return board(getID());
    board newboard = *this;
    std::swap(newboard.e[pivot/3][pivot%3], newboard.e[pivot/3 + 1][pivot % 3]);
    newboard.pivot+= 3;
    return newboard;
}
board board::moveLeft()
{
    if (pivot % 3 == 0) return board(getID());
    board newboard = *this;
    std::swap(newboard.e[pivot/3][pivot%3], newboard.e[pivot/3][pivot % 3 - 1]);
    newboard.pivot--;
    return newboard;
}
board board::moveRigh()
{
    if (pivot % 3 == 2) return board(getID());
    board newboard = *this;
    std::swap(newboard.e[pivot/3][pivot%3], newboard.e[pivot/3][pivot % 3 + 1]);
    newboard.pivot++;
    return newboard;
}
int board::getID()
{
    int n = 0;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            n = n * 10 + e[i][j];
    return n;
}
