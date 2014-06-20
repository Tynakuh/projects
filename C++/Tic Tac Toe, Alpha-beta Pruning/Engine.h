#include <stdlib.h>
#include <iostream>
#include "Gui.h"
#include "Ai.h"

using namespace std;

class Engine{
    public:
        //Functions
        Engine();
        void start(bool option); //Start game/engine. Option is mode. 0 = Player vs Player, 1 = Player vs Computer
        
    private:
        //Attributes
        bool player; //Player for next call of turn.
        char board[3][3];
        bool playing;
        bool mode;
        Ai a;
        Gui g; 
        //Functions
        void turn(); //One turn.
        int getMove();
        void updateBoard(int move);
        char checkWin(char board[3][3]);
        
};
