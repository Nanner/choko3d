#include "Game.h"

BoardPiece::BoardPiece(unsigned int id): id(id), onBoard(false), playable(true), toggled(false) {}

//PickingSquare::PickingSquare(unsigned int id): id(id), pieceID(0), hasPiece(false) {}

void Game::loadBoardPiecesPositions() {
	//Load Player 1 pieces
	unsigned int p1ID = 1 + NUMBER_OF_SQUARE_COLUMNS * NUMBER_OF_SQUARE_ROWS;
	unsigned int row = 0;
	unsigned int column = 0;
	for(row = 0; row < NUMBER_OF_PIECE_ROWS - 1; row++) {
		for(column = 0; column < NUMBER_OF_PIECE_COLUMNS; column++) {
			PositionPoint position;
			position.x = (float) FIRST_P1PIECE_POSITION_X + (float) column * (float) SPACE_BETWEEN_PIECES;
			position.y = (float) FIRST_P1PIECE_POSITION_Y;
			position.z = (float) FIRST_P1PIECE_POSITION_Z + (float) row * (float) SPACE_BETWEEN_PIECES;
			printf("x= %f, y= %f, z= %f\n", position.x, position.y, position.z);
			boardPiecesInitialPositions.insert(pair<unsigned int, PositionPoint>(p1ID, position));
			boardPieces.find(p1ID)->second->position[0] = position.x;
			boardPieces.find(p1ID)->second->position[1] = position.y;
			boardPieces.find(p1ID)->second->position[2] = position.z;
			p1ID++;
		}
	}
	//On the last row, we want two pieces "centered" in front of the others
	PositionPoint p1position1 = {(float) FIRST_P1PIECE_POSITION_X + (float) SPACE_BETWEEN_PIECES, (float) FIRST_P1PIECE_POSITION_Y, (float) FIRST_P1PIECE_POSITION_Z + (float) row * (float) SPACE_BETWEEN_PIECES};
	boardPiecesInitialPositions.insert(pair<unsigned int, PositionPoint>(p1ID, p1position1));
	boardPieces.find(p1ID)->second->position[0] = p1position1.x;
	boardPieces.find(p1ID)->second->position[1] = p1position1.y;
	boardPieces.find(p1ID)->second->position[2] = p1position1.z;
	p1ID++;

	PositionPoint p1position2 = {(float) FIRST_P1PIECE_POSITION_X + 3.0 * (float) SPACE_BETWEEN_PIECES, (float) FIRST_P1PIECE_POSITION_Y, (float) FIRST_P1PIECE_POSITION_Z + (float) row * (float) SPACE_BETWEEN_PIECES};
	boardPiecesInitialPositions.insert(pair<unsigned int, PositionPoint>(p1ID, p1position2));
	boardPieces.find(p1ID)->second->position[0] = p1position2.x;
	boardPieces.find(p1ID)->second->position[1] = p1position2.y;
	boardPieces.find(p1ID)->second->position[2] = p1position2.z;

	//Load Player 2 pieces
	unsigned int p2ID = NUMBER_OF_PLAYER_PIECES + 1 + NUMBER_OF_SQUARE_COLUMNS * NUMBER_OF_SQUARE_ROWS;
	for(row = NUMBER_OF_PIECE_ROWS; row > 1; row--) {
		for(column = 0; column < NUMBER_OF_PIECE_COLUMNS; column++) {
			PositionPoint position = {(float) FIRST_P2PIECE_POSITION_X + (float) column * (float) (float) SPACE_BETWEEN_PIECES, (float) FIRST_P2PIECE_POSITION_Y, (float) FIRST_P2PIECE_POSITION_Z + (float) row * (float) SPACE_BETWEEN_PIECES};
			boardPiecesInitialPositions.insert(pair<unsigned int, PositionPoint>(p2ID, position));
			boardPieces.find(p2ID)->second->position[0] = position.x;
			boardPieces.find(p2ID)->second->position[1] = position.y;
			boardPieces.find(p2ID)->second->position[2] = position.z;
			p2ID++;
		}
	}
	//On the last row, we want two pieces "centered" in front of the others
	PositionPoint p2position1 = {(float) FIRST_P2PIECE_POSITION_X + (float) SPACE_BETWEEN_PIECES, (float) FIRST_P2PIECE_POSITION_Y, (float) FIRST_P2PIECE_POSITION_Z + ((float) NUMBER_OF_PIECE_ROWS - 1) * (float) SPACE_BETWEEN_PIECES};
	boardPiecesInitialPositions.insert(pair<unsigned int, PositionPoint>(p2ID, p2position1));
	boardPieces.find(p2ID)->second->position[0] = p2position1.x;
	boardPieces.find(p2ID)->second->position[1] = p2position1.y;
	boardPieces.find(p2ID)->second->position[2] = p2position1.z;
	p2ID++;

	PositionPoint p2position2 = {(float) FIRST_P2PIECE_POSITION_X + 3.0 * (float) SPACE_BETWEEN_PIECES, (float) FIRST_P2PIECE_POSITION_Y, (float) FIRST_P2PIECE_POSITION_Z + (float) row * (float) SPACE_BETWEEN_PIECES};
	boardPiecesInitialPositions.insert(pair<unsigned int, PositionPoint>(p2ID, p2position2));
	boardPieces.find(p2ID)->second->position[0] = p2position2.x;
	boardPieces.find(p2ID)->second->position[1] = p2position2.y;
	boardPieces.find(p2ID)->second->position[2] = p2position2.z;

	map<unsigned int, PositionPoint>::iterator it = boardPiecesInitialPositions.begin();
	printf("size: %u\n", boardPiecesInitialPositions.size());
	for(; it != boardPiecesInitialPositions.end(); it++) {
		printf("Piece id: %u, x = %f, y = %f, z = %f\n", it->first, it->second.x, it->second.y, it->second.z);
	}
}

void Game::loadPickingSquaresPositions() {
	
	unsigned int squareID = 1;
	for(unsigned int row = 0; row < NUMBER_OF_SQUARE_ROWS; row++) {
		for(unsigned int column = 0; column < NUMBER_OF_SQUARE_COLUMNS; column++) {
			PositionPoint position;
			position.x = (float) FIRST_SQUARE_POSITION_X + (float) column * (float) SPACE_BETWEEN_PIECES;
			position.y = (float) FIRST_SQUARE_POSITION_Y;
			position.z = (float) FIRST_SQUARE_POSITION_Z + (float) row * (float) SPACE_BETWEEN_PIECES;
			pickingSquaresPositions.insert(pair<unsigned int, PositionPoint>(squareID, position));
			squareID++;
		}
	}
}

Game::Game() {
	selectState = SELECT_ANY;
}

void Game::addPiece(BoardPiece* piece) {
	boardPieces.insert(pair<unsigned int, BoardPiece*>(piece->id, piece));
}

/*void Game::addPickingSquare(PickingSquare* square) {
	pickingSquares.insert(pair<unsigned int, PickingSquare*>(square->id, square));
}*/

int Game::getPieceID(string idStr) {
	/* Attention: Pieces range from 26 to 49 */

	unsigned int id = strtoul(idStr.c_str(), NULL, 10);
	if(id != 0L && id != UINT_MAX) {
		map<unsigned int, BoardPiece*>::iterator it = boardPieces.find(id + NUMBER_OF_SQUARE_COLUMNS * NUMBER_OF_SQUARE_ROWS);
		if(it != boardPieces.end())
			id += (NUMBER_OF_SQUARE_COLUMNS * NUMBER_OF_SQUARE_ROWS);
			return(id);
	}

	return -1;
}

PositionPoint Game::getSelectedSquarePosition() {
	map<unsigned int, PositionPoint>::iterator it = pickingSquaresPositions.find(selectedPieceID);
	if(it != pickingSquaresPositions.end()) {
		return it->second;
	}

	//If not found, return point 0 0 0.
	PositionPoint p = {0.0, 0.0, 0.0};
	return p;
}

int Game::getSelectState() {
	return selectState;
}

void Game::setSelectState(int selectState) {
	this->selectState = selectState;
}

int Game::getPieceWithPosition(PositionPoint position) {
	map<unsigned int, BoardPiece*>::iterator it = boardPieces.begin();

	for(; it != boardPieces.end(); it++) {
		if(it->second->position[0] == position.x && it->second->position[1] == position.y && it->second->position[3] == position.z) {
			return it->first;
		}
	}

	return -1;
}

int Game::pickingSquareHasPiece(unsigned int squareID) {
	map<unsigned int, PositionPoint>::iterator it = pickingSquaresPositions.find(squareID);
	PositionPoint position;
	if(it == pickingSquaresPositions.end())
		return -1;
	else {
		position = it->second;
	}
	return(getPieceWithPosition(position));
}

bool Game::isBoardPiece(unsigned int id) {
	map<unsigned int, BoardPiece*>::iterator it = boardPieces.find(id);
	return!(it == boardPieces.end());
}

bool Game::canMoveTo(unsigned int squareID) {
	return true;
}