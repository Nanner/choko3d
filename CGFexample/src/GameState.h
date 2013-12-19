#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>

using namespace std;

class Move {
public:
    int fromSquare;
    int toSquare;
    int firstAttackSquare;
    int secondAttackSquare;
    Move(int fromSquare = 0, int toSquare = 0, int firstAttackSquare = 0, int secondAttackSquare = 0);
};

class GameState {
    
public:
    vector<string> board;
    char currentPlayer;
    int currentPlayerUnusedPieces;
    int enemyPlayerUnusedPieces;
    int player1UnusedPieces;
    int player2UnusedPieces;
    char dropInitiative;
    string move;
    vector<int> removedPieces;
    char winner;
    bool gameOver;
    Move parsedMove;
    Move getMove();
    
    GameState(string stateString);
    
    GameState(string stateString, GameState previousState);
    
    static string toString(vector<string> board);
    static vector<string> toVector(string board);
    static vector<vector<int> > movesToVector(string movesString);
    static int getFirstAttack(int from, int to);
};

#endif
