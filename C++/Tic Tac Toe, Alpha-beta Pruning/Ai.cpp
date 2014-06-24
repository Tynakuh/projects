#include <cstdlib>
#include <iostream>
#include "Ai.h"

Ai::Ai(){
    aiPlayer = 'X';
    huPlayer = 'O';
}

int Engine::getMove(char board[3][3]){
    int score = -10000;
    int move = NULL;
    score = max(score,getMin(board,-10000,10000,&move));
    return move;
}



int Engine::getMax(char board[3][3], int alpha, int beta, int* move = NULL){
    
    
    int score = 0;
    int bestscore = -10000;
    for (int i=0; i<3;i++){
        char tempBoard[3][3];
        tempBoard = copyBoard(board);
        for(int j=0;j<3;j++){
            if(board[i][j]!='X'&&board[i][j]!='O'){//For every empty position
                tempBoard[i][j] = aiPlayer;
                score = getMin(tempBoard,alpha,beta);
                if(score>= bestscore){ //get Max
                    bestscore = score;
                    move = ((i*3)+j);
                }
                if(score>=beta){return score;}; //Prune tree.
                alpha = max(alpha,score);
            }
        }
    }
    //Full board past this
    return getScore(board);
}

int Engine::getMin(char board[3][3], int alpha, int beta, int* move = NULL){
    int score = 0;
    int bestscore = 10000
    for (int i=0; i<3;i++){
        char tempBoard[3][3];
        int tempScore = 0;
        tempBoard = copyBoard(board);
        for(int j=0;j<3;j++){
            if(board[i][j]!='X'&&board[i][j]!='O'){//For every empty position
                tempBoard[i][j] = huPlayer;
                score = getMax(tempBoard,alpha,beta);
                if(score<=bestscore){ //get Min
                    bestscore = score;
                    move = ((i*3)+j);
                }
                if(score<=alpha){return bestscore;}; //Prune tree.
                alpha = min(beta,bestscore);
            }
        }
    }
    //Full board past this
    return getScore(board);
}

int Engine::getScore(char board[3][3]){ //Get heuristic,  +10 for 3 Xs in a line, +1 for 2 Xs in a line, -10 for 3 Os in a line, -1 for 2 Os in a line. 
        int score = 0;
        int xCount = 0;
        int oCount = 0;
        
        for(int i = 0 ; i <3 ; i++){ //Check rows for three of the same value (win for a player)
            for(int j = 0; j<3; j++){
                if(board[i][j] == 'X'){
                    xCount++;
                }
                if(board[i][j] == 'O'){
                    oCount++;
                }
            }
            score = score + getScore(xCount,oCount);
            xCount = 0;
            oCount = 0;
        }
        
        for(int i = 0 ; i <3 ; i++){ //Check columns for three of the same value (win for a player)
            for(int j = 0; j<3; j++){
                if(board[j][i] == 'X'){
                    xCount++;
                }
                if(board[j][i] == 'O'){
                    oCount++;
                }
            }
            score = score + getScore(xCount,oCount);
            xCount = 0;
            oCount = 0;
        }
        for(int i = 0; i<3 ; i++){//Topleft to bottomright diagonal
            if(board[i][i] == 'X'){
                xCount++;
            }
            if(board[i][i] == 'O'){
                oCount++;
            }
        }
        score = score + getScore(xCount,oCount);
        xCount = 0;
        oCount = 0;
        
        for(int i = 0; i<3;i++){ //Bottomleft to Topright diagonal
            if(board[i][2-i] == 'X'){
                xCount++;
            }
            if(board[i][2-i] == 'O'){
                oCount++;
            }
        }
        score = score + getScore(xCount,oCount);
        del xCount;
        del oCount;
        return score;
}

char Engine::copyBoard(char board[3][3]){
    char newBoard[3][3];
    for(int i = 0; i<3;i++){
        for(int j =0; j<3;j++){
            newBoard[i][j] = board[i][j];
        }
    }
    return newBoard;
}

int Engine::countScore(int xCount, oCount){
    int score = 0;
    if(xCount==3){
        score = score + 10;
    }
    if(xCount==2){
        score = score + 1;
    }
    if(oCount==3){
        score = score -10;
    }
    if(oCount==2){
        score = score -1;
    }
}
