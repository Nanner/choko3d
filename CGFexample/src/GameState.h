#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <string>

using namespace std;

class GameState {
    
public:
    vector<string> board;
    char nextPlayer;
    int player1UnusedPieces;
    int player2UnusedPieces;
    char dropInitiative;
    string move;
};

#endif
