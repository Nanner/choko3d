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

class PrologBridge {
private:
    Connection *con;
    void initialize(int);
public:
    static short port;
    PrologBridge();
    PrologBridge(int);
    string initializeGame();
    
#ifdef linux
    static pid_t procId;
#endif
    GameState execute(string move, vector<string> board, char player, int playerUnusedPieces, int enemyUnusedPieces, char dropInitiative);
    
    GameState calculate(vector<string> board, char player, int playerUnusedPieces, int enemyUnusedPieces, char dropInitiative, string playerDifficulty);
    
    string gameOver(vector<string> board, char player, int playerUnusedPieces, int enemyUnusedPieces);
    
};

#endif
