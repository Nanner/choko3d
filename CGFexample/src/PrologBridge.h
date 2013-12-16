#ifndef _GRAPH_VIEWER_H_
#define _GRAPH_VIEWER_H_

#ifdef __APPLE__
#define linux
#endif

#ifdef linux
#include <unistd.h>
#define CHOKO "./choko"
#else
#include <winsock2.h>
#include <Windows.h>
#define CHOKO "choko.exe"
#endif

#include <stdlib.h>
#include <signal.h>
#include <string>
#include <vector>

#include "connection.h"
#include "GameState.h"

class PieceMoves {
public:
    vector<int> moves;
    vector<int> attacks;
    vector<int> targets;
    PieceMoves(vector<int> moves, vector<int> attacks, vector<int> targets): moves(moves), attacks(attacks), targets(targets) {}
};

class PrologBridge {
private:
    Connection *con;
    void initialize(int);
public:
    static short port;
    PrologBridge();
    PrologBridge(int);
    
#ifdef linux
    static pid_t procId;
#endif
    GameState initializeGame();
    
    GameState execute(GameState gamestate, string move);
    
    GameState calculate(GameState gameState, string playerDifficulty);
    
    GameState checkGameOver(GameState gameState);
    
    PieceMoves getPieceMoves(GameState gameState, int position);
};

class InvalidMove {};

#endif
