#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>

using namespace std;

class GameState {

public:
    vector<string> board;
    char nextPlayer;
    int player1UnusedPieces;
    int player2UnusedPieces;
    char dropInitiative;
    string move;

    GameState(string);
    
    static string toString(vector<string> board);
    static vector<string> toVector(string board);
};

#endif
