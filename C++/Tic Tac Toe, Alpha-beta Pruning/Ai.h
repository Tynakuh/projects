#include <stdlib.h>
#include <iostream>
using namespace std;

class Ai{
    public:
        int getMove(char board[3][3]);
    private:
        char aiPlayer;
        int getScore(char board[3][3]);
        int countScore(int xScore, int oScore);
        char copyBoard(char board[3][3]);
        int count;
};
