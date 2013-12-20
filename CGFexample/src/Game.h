#ifndef GAME_H
#define GAME_H

#include <map>
#include <limits.h>
#include <string>
#include <stdlib.h>
#include <vector>
#include <stack>

#include "PrologBridge.h"

#define NUMBER_OF_PLAYER_PIECES 12

#define SPACE_BETWEEN_PIECES 4

#define NUMBER_OF_PIECE_ROWS 3
#define NUMBER_OF_PIECE_COLUMNS 5

#define NUMBER_OF_SQUARE_ROWS 5
#define NUMBER_OF_SQUARE_COLUMNS 5
#define NUMBER_OF_SQUARES NUMBER_OF_SQUARE_ROWS * NUMBER_OF_SQUARE_COLUMNS

#define FIRST_P1REST_POS_X -15.0
#define FIRST_P1REST_POS_Y 0.0
#define FIRST_P1REST_POS_Z 2.0

#define FIRST_P2REST_POS_X 35.0
#define FIRST_P2REST_POS_Y 0.0
#define FIRST_P2REST_POS_Z 2.0

#define FIRST_P2PIECE_POSITION_X 2.0
#define FIRST_P2PIECE_POSITION_Y 0.0
#define FIRST_P2PIECE_POSITION_Z -15.0

#define FIRST_P1PIECE_POSITION_X 2.0
#define FIRST_P1PIECE_POSITION_Y 0.0
#define FIRST_P1PIECE_POSITION_Z 35.0

#define FIRST_SQUARE_POSITION_X 2.0
#define FIRST_SQUARE_POSITION_Y 0.0
#define FIRST_SQUARE_POSITION_Z 2.0

#define SELECT_ANY 0
#define SELECT_TO_SQUARE 1
#define SELECT_SECOND_ENEMY 2

#define HUMAN 0
#define EASY 1
#define MEDIUM 2
#define HARD 3

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
    unsigned int squareID;
    char player;
    char getOpponent(){
        if (this->player == 'x')
            return 'o';
        else return 'x';
    };
};

class Game {
private:
	map<unsigned int, BoardPiece*> boardPieces;
	//map<unsigned int, PickingSquare*> pickingSquares;
	map<unsigned int, PositionPoint> boardPiecesInitialPositions;
	map<unsigned int, PositionPoint> pickingSquaresPositions;
	int selectState;
    PrologBridge choko;
    stack<GameState> gameStates;
	stack<PositionPoint> p1RestPositions;
	stack<PositionPoint> p2RestPositions;
    PositionPoint firstAttackingOrigin;
    PositionPoint firstAttackingDestination;
    static string playerTypes[4];

public:
    int player1Type;
    int player2Type;
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
    BoardPiece * getBoardPiece(unsigned int pieceID);
	PositionPoint getBoardPiecePosition(unsigned int pieceID);
	PositionPoint getPickingSquarePosition(unsigned int squareID);
    int getPickingSquareID(PositionPoint position);
	void setBoardPiecePosition(unsigned int pieceID, PositionPoint position);
    int getPieceOnSquare(int squareID);
    
    GameState getGameState();
	char getCurrentPlayer();
    void setBoardPieceSquare(unsigned int pieceID, unsigned int squareID);
    int executeMove(int pieceID, PositionPoint destination);
    int executeMove(PositionPoint firstAttackingOrigin, PositionPoint firstAttackingDestination, int secondEnemyPieceID);
	bool isOwnPiece(int pieceID);
    int getPiecesOnBoard(char player);
	bool canCapture(int pieceID);
	void capture(int secondPieceID);

	PositionPoint getNextP1RestPosition();
	PositionPoint getNextP2RestPosition();
	PositionPoint getPieceRestPosition(BoardPiece* piece);

	PositionPoint getSecondP1RestPosition();
	PositionPoint getSecondP2RestPosition();
	PositionPoint getSecondPieceRestPosition(BoardPiece* piece);

	void popPieceRestPosition(BoardPiece* piece);

	bool currentPlayerIsAI();

	bool hasGameEnded();
	int getWinner();
    
    void update();
    int calculateMove(int playerType);
	void processAIMovedPieces(Move move);
	BoardPiece* getUnusedPiece(char player);
};

#endif