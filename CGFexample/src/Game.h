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

#define AI_WAIT_AFTER_UNDO 1500

using namespace std;

struct PositionPoint {
	float x;
	float y;
	float z;
};

class MovementHistoryElement {
public:
	int moveType;
	vector<int> modifiedPieces;
	MovementHistoryElement(int moveType, int movedPiece, int firstCapturedPiece, int secondCapturedPiece);
};

class BoardPiece {
public:
	stack<float*> previousPositions;
	stack<int> previousSquares;
	BoardPiece(unsigned int id);
	unsigned int id;
	float position[3];
	bool onBoard;
	bool playable;
	bool toggled;
    unsigned int squareID;
    char player;
    char getOpponent();
	void resetPiece();
	void undoMovement();
};

class Game {
private:
	map<unsigned int, BoardPiece*> boardPieces;
	map<unsigned int, PositionPoint> boardPiecesInitialPositions;
	map<unsigned int, PositionPoint> pickingSquaresPositions;
	int selectState;
    PrologBridge choko;
    stack<GameState> gameStates;
	stack<PositionPoint> p1RestPositions;
	stack<PositionPoint> p2RestPositions;
	int firstCapturedPieceID;
    PositionPoint firstAttackingOrigin;
    PositionPoint firstAttackingDestination;
    static string playerTypes[4];
	stack<MovementHistoryElement> movementHistory;
public:
    bool calculatedMovesForPlayerTurn;
    bool movesPossible;
    bool AIisStandingBy;
    unsigned long AIStandByStart;
    float turnTimeLeft;
    unsigned long time;
    float timeout;
    unsigned long turnStart;
	int player1Type;
	int player2Type;
	unsigned int selectedPieceID;

	Game();
	~Game();
	void restartGame();
	GameState getGameState();
	stack<GameState> getGameStates();
	void addGameState(GameState state);
	int executeMove(int pieceID, PositionPoint destination);
	int executeMove(PositionPoint firstAttackingOrigin, PositionPoint firstAttackingDestination, int secondEnemyPieceID);
	bool canMoveTo(unsigned int squareID);
	bool canCapture(int pieceID);
	void capture(int secondPieceID);

	void loadBoardPiecesPositions();
	void loadPickingSquaresPositions();

	int getSelectState();
	void setSelectState(int selectState);

	void addPiece(BoardPiece* piece);
	int getPieceID(string idStr);
	string getPieceIDStr(unsigned int id);
	int getPieceWithPosition(PositionPoint position);
	bool isBoardPiece(unsigned int id);
	BoardPiece * getBoardPiece(unsigned int pieceID);
	PositionPoint getBoardPiecePosition(unsigned int pieceID);
	void setBoardPiecePosition(unsigned int pieceID, PositionPoint position);
	void setBoardPieceSquare(unsigned int pieceID, int squareID);
	int getPieceOnSquare(int squareID);
	bool isOwnPiece(int pieceID);
	int getPiecesOnBoard(char player);

	PositionPoint getSelectedSquarePosition();
	int pickingSquareHasPiece(unsigned int squareID);
	PositionPoint getPickingSquarePosition(unsigned int squareID);
    int getPickingSquareID(PositionPoint position);
    
	char getCurrentPlayer();

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
    
    void updateAI();
    int calculateMove(int playerType);
	void processAIMovedPieces(Move move);
	BoardPiece* getUnusedPiece(char player);

	MovementHistoryElement getLastMove();
	void undoLastMove();

    void update(unsigned long t);
    void skipTurn();
};

#endif