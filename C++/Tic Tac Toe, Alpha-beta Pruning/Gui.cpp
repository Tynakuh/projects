#include <cstdlib>
#include <iostream>
#include "GUI.h"

        
Gui::Gui(){
}

void Gui::DisplayBoard(char board[3][3]){  //Display the current state of the Board. 
    cout<<string(100,'\n'); //Clear the screen.
    for(int i=0; i<3;i++){ //Loop through rows
        cout<<"|";
        for(int j=0;j<3;j++){ //Loop through columns
            cout<<board[i][j]<<"|";
        }
        cout<<"\n";
    }
}
