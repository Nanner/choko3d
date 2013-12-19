#include "GameState.h"

Move::Move(char player, int moveType, int fromSquare, int toSquare, int firstAttackSquare, int secondAttackSquare): fromSquare(fromSquare),
toSquare(toSquare),
firstAttackSquare(firstAttackSquare),
secondAttackSquare(secondAttackSquare),
moveType(moveType),
player(player)
{}

GameState::GameState(string stateString) {
	initialize(stateString);
}

GameState::GameState(string stateString, GameState previousState) {
	initialize(stateString);
    // string move examples:
    // 5 -> drop
    // 5-3 -> move
    // 5-3-4 -> attack
    int fromSquare = 0;
    int toSquare = 0;
    int firstAttackSquare = 0;
    int secondAttackSquare = 0;
	int moveType = 0;
	char player = previousState.currentPlayer;
    
    int scanned = sscanf(this->move.c_str(), "%d-%d-%d", &fromSquare, &toSquare, &secondAttackSquare);
    
    if ( scanned < 1 ) printf("Parsing error\n");
    
    if ( scanned == 1 ) {
        // move was a drop
        // scanf parsed fromSquare, but we want it as toSquare
        toSquare = fromSquare;
        fromSquare = 0;
		moveType = DROP;
    }

	if(scanned == 2) {
		moveType = MOVE;
	}
    
    if (scanned == 3) {
        firstAttackSquare = getFirstAttack(fromSquare, toSquare);
		moveType = ATTACK;
    }
    
    this->parsedMove = Move(player, moveType, fromSquare, toSquare, firstAttackSquare, secondAttackSquare);
    printf("new move\n");
    
}



void GameState::initialize( string &stateString )
{
	// example string
	// "[[1,2,3,4,x,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25],o,12,11,x,5]"
	printf("State string: %s\n", stateString.c_str());
	istringstream ss(stateString);
	ss.ignore(1, '[');
	string boardString;
	getline(ss, boardString, ']'); boardString += ']';
	this->board = toVector(boardString);

	ss.ignore(1, ',');
	string nextPlayerStr;
	getline(ss, nextPlayerStr, ',');
	this->currentPlayer = nextPlayerStr.at(0);

	string playerUnusedPieces;
	getline(ss, playerUnusedPieces, ',');

	string enemyUnusedPieces;
	getline(ss, enemyUnusedPieces, ',');

	string dropInitiativeStr;
	getline(ss, dropInitiativeStr, ',');
	this->dropInitiative = dropInitiativeStr.at(0);

	getline(ss, this->move, ']');

	if (currentPlayer == 'x') {
		this->player1UnusedPieces = atoi(playerUnusedPieces.c_str());
		this->currentPlayerUnusedPieces = player1UnusedPieces;
		this->player2UnusedPieces = atoi(enemyUnusedPieces.c_str());
		this->enemyPlayerUnusedPieces = player2UnusedPieces;
	} else {
		this->player2UnusedPieces = atoi(playerUnusedPieces.c_str());
		this->currentPlayerUnusedPieces = player2UnusedPieces;
		this->player1UnusedPieces = atoi(enemyUnusedPieces.c_str());
		this->enemyPlayerUnusedPieces = player1UnusedPieces;
	}

	this->winner = '0';
	this->gameOver = false;
	this->parsedMove = Move();
}


int GameState::getFirstAttack(int from, int to) {
    // up
    if (to == (from - 10))
        return from - 5;
    
    // down
    if (to == (from + 10))
        return from + 5;
    
    // left
    if (to == (from - 2) )
        return from - 1;
    
    // right
    if (to == (from + 2) )
        return from + 1;
    
    return 0;
}

Move GameState::getMove() {
    return this->parsedMove;
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
	printf("Board: %s\n", board.c_str());
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