#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <map>
#include <limits.h>
#include <string>
#include <stdlib.h>

using namespace std;

class BoardPiece {
public:
	BoardPiece(unsigned int id);
	unsigned int id;
	float position[3];
	bool onBoard;
	bool playable;
};

class GameState {
private:
	map<unsigned int, BoardPiece> boardPieces;

public:
	GameState() {};
	void addPiece(BoardPiece piece);
	int getPieceID(string idStr);

};

#endif