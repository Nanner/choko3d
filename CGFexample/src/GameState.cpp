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
        this->player2UnusedPieces = atoi(enemyUnusedPieces.c_str());
    } else {
        this->player2UnusedPieces = atoi(playerUnusedPieces.c_str());
        this->player1UnusedPieces = atoi(enemyUnusedPieces.c_str());
    }
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