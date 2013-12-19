#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>

#define DROP 1
#define MOVE 2
#define ATTACK 3

using namespace std;

class Move {
public:
	char player;
	int moveType;
    int fromSquare;
    int toSquare;
    int firstAttackSquare;
    int secondAttackSquare;
    Move(char player = ' ', int moveType = 0, int fromSquare = 0, int toSquare = 0, int firstAttackSquare = 0, int secondAttackSquare = 0);
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
	void initialize( string &stateString );

    static string toString(vector<string> board);
    static vector<string> toVector(string board);
    static vector<vector<int> > movesToVector(string movesString);
    static int getFirstAttack(int from, int to);
};

#endif
