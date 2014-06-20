#include "Engine.h"

Engine::Engine(){ //Default constructor, intialize board and player.
    for(int i = 0; i<3;i++){
        for(int j = 0; j<3; j++){
            board[i][j] = '1' + (i*3) + j; //Character value of 1 added to obtain integer values 1-9
        }
    }
    player = 1; //Setting to Player 1 (0/False is Player 2)
}

void Engine::turn(){        //Performing one turn of the game.
    int move = getMove()-1; //Getting desired move from player then
                            //Converting the range of 1-9 to 0-8 for easier manipulation of arrays
    int row = move/3;       //Getting the row number by dividing by 3 (0,1,2/3 = 0) (3,4,5 / 3 = 1) etc.
    if(player){             //If Player 1
        board[row][move-(row*3)] = 'X';
    }
    else{                   //Else Player 2
        board[row][move-(row*3)] = 'O';
    }
}

char Engine::checkWin(char board[3][3]){ //Check for end of game state
    for(int i = 0 ; i <3 ; i++){ //Check rows for three of the same value (win for a player)
    if(board[i][0]==board[i][1] && board[i][1]==board[i][2]){
        return board[i][0]; //Return winner character
    }}
    for(int i = 0 ; i <3 ; i++){ //Check coulmns
    if(board[0][i]==board[1][i] && board[1][i]==board[2][i]){
        return board[0][i]; //Return winner character
    }}
    
    //***More efficient to check diagonals first
    
    if(board[0][0]==board[1][1] && board[1][1]==board[2][2]){ //Check topleft to bottomright diagonal
        return board[0][0]; //Return winner character
    }
    if(board[0][2]==board[1][1] && board[1][1]==board[2][0]){ //Check topright to bottomleft diagonal
        return board[0][2]; //Return winner character
    }
    
    for(int i =0;i<3;i++){ //Check for tie, board filled without winner.
        for(int j=0;j<3;j++){
        if(board[i][j]!='X' && board[i][j] !='O'){
            return 'C'; //Return continue character (C) as board is not full.
        }}
    }
    return 'T'; //Return Tie character (T), board is full with no winner.
}

int Engine::getMove(){ //Get player or AI's move.
    int move;
    if(mode && !player){ //If AI-mode is on, and player is Player 2 (AI)
        move = a.getMove();
    }
    else{
        cout<<"Input move:\n";
        while(1){
            cin>>move;//*** :IMPLEMENT: *** Implement a more robust error detection and valid input check.
            if(move>0 && move<10){ //Ensures a valid number is input, ignores anything else. ***Error message?
                 break;
            }
        }
    }
    return move;
}


void Engine::start(bool option){ //Initialize game
    g.DisplayBoard(board);
    
    while(checkWin(board) == 'C'){ //Check every loop if game is at an end state (winner or tie)
        turn(); //Play a turn.
        g.DisplayBoard(board); //Display the board to user.
        player = !player; //Change player.
    }
    //Game is over.
    //Check winner and display appropriate message to inform user.
    char winner = checkWin(board);
    if(winner=='T'){
        cout<<"It's a tie!";
    }
    else{
        cout<<"The winner is: " << winner;
    }
    int temp; // :IMPLEMENT: ***Temporary GUI solution until proper GUI is implemented***
    cin>>temp; //Maintain window open till value is input.
}

