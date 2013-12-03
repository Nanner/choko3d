#ifndef GAME_H
#define GAME_H

#include <map>
#include <limits.h>
#include <string>
#include <stdlib.h>
#include <vector>

#define NUMBER_OF_PLAYER_PIECES 12

#define SPACE_BETWEEN_PIECES 4

#define NUMBER_OF_PIECE_ROWS 3
#define NUMBER_OF_PIECE_COLUMNS 5

#define NUMBER_OF_SQUARE_ROWS 5
#define NUMBER_OF_SQUARE_COLUMNS 5

#define FIRST_P1PIECE_POSITION_X 2.0
#define FIRST_P1PIECE_POSITION_Y 0.0
#define FIRST_P1PIECE_POSITION_Z -15.0

#define FIRST_P2PIECE_POSITION_X 2.0
#define FIRST_P2PIECE_POSITION_Y 0.0
#define FIRST_P2PIECE_POSITION_Z 35.0

#define FIRST_SQUARE_POSITION_X 2.0
#define FIRST_SQUARE_POSITION_Y 0.0
#define FIRST_SQUARE_POSITION_Z 2.0

#define SELECT_ANY 0
#define SELECT_TO_SQUARE 1

using namespace std;

struct PositionPoint {
	float x;
	float y;
	float z;
};

/*class PickingSquare {
public:
	PickingSquare(unsigned int id);
	unsigned int id;
	unsigned int pieceID;
	float position[3];
	bool hasPiece;
};*/

class BoardPiece {
public:
	BoardPiece(unsigned int id);
	unsigned int id;
	float position[3];
	bool onBoard;
	bool playable;
	bool toggled;
};

class Game {
private:
	map<unsigned int, BoardPiece*> boardPieces;
	//map<unsigned int, PickingSquare*> pickingSquares;
	map<unsigned int, PositionPoint> boardPiecesInitialPositions;
	map<unsigned int, PositionPoint> pickingSquaresPositions;
	int selectState;

public:
	unsigned int selectedPieceID;
	void loadBoardPiecesPositions();
	void loadPickingSquaresPositions();
	Game();
	void addPiece(BoardPiece* piece);
	//void addPickingSquare(PickingSquare* square);
	int getPieceID(string idStr);
	string getPieceIDStr(unsigned int id);
	PositionPoint getSelectedSquarePosition();
	int getSelectState();
	void setSelectState(int selectState);
	int getPieceWithPosition(PositionPoint position);
	int pickingSquareHasPiece(unsigned int squareID);
	bool isBoardPiece(unsigned int id);
	bool canMoveTo(unsigned int squareID);
	PositionPoint getBoardPiecePosition(unsigned int pieceID);
	PositionPoint getPickingSquarePosition(unsigned int squareID);
	void setBoardPiecePosition(unsigned int pieceID, PositionPoint position);
};

#endif