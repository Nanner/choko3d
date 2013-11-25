#include "GameState.h"

BoardPiece::BoardPiece(unsigned int id): id(id), onBoard(false), playable(true) {}

void GameState::addPiece(BoardPiece piece) {
	boardPieces.insert(pair<unsigned int, BoardPiece>(piece.id, piece));
}

int GameState::getPieceID(string idStr) {
	unsigned int id = strtoul(idStr.c_str(), NULL, 10);
	if(id != 0L && id != UINT_MAX) {
		map<unsigned int, BoardPiece>::iterator it = boardPieces.find(id);
		if(it != boardPieces.end())
			return(id);
	}

	return -1;
}