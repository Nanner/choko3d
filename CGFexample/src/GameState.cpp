#include "GameState.h"

GameState::GameState(string stateString) {
    // example string
    // "[[1,2,3,4,x,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25],o,12,11,x,5]"

    istringstream ss(stateString);
    ss.ignore(1, '[');
    string boardString;
    getline(ss, boardString, ']'); boardString += ']';
    this->board = toVector(boardString);
    
    ss.ignore(1, ',');
    string nextPlayerStr;
    getline(ss, nextPlayerStr, ',');
    this->nextPlayer = nextPlayerStr.at(0);
    
    string playerUnusedPieces;
    getline(ss, playerUnusedPieces, ',');
    
    string enemyUnusedPieces;
    getline(ss, enemyUnusedPieces, ',');
    
    string dropInitiativeStr;
    getline(ss, dropInitiativeStr, ',');
    this->dropInitiative = dropInitiativeStr.at(0);
    
    getline(ss, this->move, ']');
    
    if (nextPlayer == 'x') {
        this->player1UnusedPieces = atoi(playerUnusedPieces.c_str());
        this->nextPlayerUnusedPieces = player1UnusedPieces;
        this->player2UnusedPieces = atoi(enemyUnusedPieces.c_str());
        this->enemyPlayerUnusedPieces = player2UnusedPieces;
    } else {
        this->player2UnusedPieces = atoi(playerUnusedPieces.c_str());
        this->nextPlayerUnusedPieces = player2UnusedPieces;
        this->player1UnusedPieces = atoi(enemyUnusedPieces.c_str());
        this->enemyPlayerUnusedPieces = player1UnusedPieces;
    }
    
    this->winner = '0';
    this->gameOver = false;
}

vector<vector<int> > GameState::movesToVector(string movesString) {
    vector<vector<int> > movesAttacksEnemies;
    stringstream ss(movesString);
    ss.ignore(1, '[');
    string list;
    while (getline(ss, list, ']')) {
        list.push_back(']');
        vector<string> movesAsStrings = toVector(list);
        vector<int> moves;
        for (int i = 0; i < movesAsStrings.size(); i++) {
            if ( movesAsStrings.at(i).size() > 0 )
                moves.push_back(atoi(movesAsStrings.at(i).c_str()));
        }
        ss.ignore(1, ',');
        movesAttacksEnemies.push_back(moves);
    }
    movesAttacksEnemies.pop_back();
    return movesAttacksEnemies;
}

string GameState::toString(vector<string> board) {
    string boardString = "[";
    for (int i = 0; i < board.size(); i++) {
        boardString += board.at(i);
        if (i < board.size()-1)
            boardString += ",";
    }
    boardString += "]";
    return boardString;
}

vector<string> GameState::toVector(string board) {
    vector<string> boardVector;
    stringstream ss(board);
    ss.ignore(1, '[');
    string boardCell;
    while (getline(ss, boardCell, ',')) {
        if (boardCell.at(boardCell.size()-1) == ']')
            boardCell.pop_back();
        boardVector.push_back(boardCell);
    }
    return boardVector;
}