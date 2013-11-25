#ifndef GAME_H
#define GAME_H

#include <map>
#include <limits.h>
#include <string>
#include <stdlib.h>

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

using namespace std;

struct PositionPoint {
	float x;
	float y;
	float z;
};

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
	map<unsigned int, BoardPiece> boardPieces;
	map<unsigned int, PositionPoint> boardPiecesInitialPositions;
	map<unsigned int, PositionPoint> pickingSquaresPositions;

public:
	unsigned int selectedPieceID;
	void loadBoardPiecesPositions();
	void loadPickingSquaresPositions();
	Game();
	void addPiece(BoardPiece piece);
	int getPieceID(string idStr);
	PositionPoint getSelectedSquarePosition();

};

#endif