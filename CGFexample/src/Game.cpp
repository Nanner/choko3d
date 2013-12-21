#include "Game.h"

MovementHistoryElement::MovementHistoryElement(int moveType, int movedPiece, int firstCapturedPiece, int secondCapturedPiece) {
	this->moveType = moveType;
	
	if(movedPiece != 0)
		modifiedPieces.push_back(movedPiece);

	if(firstCapturedPiece != 0)
		modifiedPieces.push_back(firstCapturedPiece);

	if(secondCapturedPiece != 0)
		modifiedPieces.push_back(secondCapturedPiece);
}

BoardPiece::BoardPiece(unsigned int id): id(id), onBoard(false), playable(true), toggled(false), squareID(0) {}

void BoardPiece::resetPiece() {
	onBoard = false;
	playable = true;
	toggled = false;
	squareID = 0;
}

char BoardPiece::getOpponent() {
	if (this->player == 'x')
		return 'o';
	else return 'x';
}

void BoardPiece::undoMovement() {
	float* previousPosition = previousPositions.top();
	previousPositions.pop();

	position[0] = previousPosition[0];
	position[1] = previousPosition[1];
	position[2] = previousPosition[2];
	delete previousPosition;

	squareID = previousSquares.top();
	previousSquares.pop();
}

string Game::playerTypes[] = {"human", "easy", "medium", "hard"};

void Game::restartGame() {
	map<unsigned int, BoardPiece*>::iterator it = boardPieces.begin();
	for(; it != boardPieces.end(); it++) {
		it->second->resetPiece();
	}

	while(!p1RestPositions.empty())
		p1RestPositions.pop();

	while(!p2RestPositions.empty())
		p2RestPositions.pop();

	loadBoardPiecesPositions();

	while(!gameStates.empty())
		gameStates.pop();

	selectState = SELECT_ANY;

	player1Type = HARD;
	player2Type = HARD;

	try {
		GameState gameState = choko.initializeGame();
		gameStates.push(gameState);
	} catch (InvalidMove &invalid) {
		printf("Initialization error.\n");
	}
}

void Game::loadBoardPiecesPositions() {
	//Load Player 1 pieces
	unsigned int p1ID = 1 + NUMBER_OF_SQUARE_COLUMNS * NUMBER_OF_SQUARE_ROWS;
	unsigned int row = 0;
	unsigned int column = 0;
	for(row = NUMBER_OF_PIECE_ROWS; row > 1; row--) {
		for(column = 0; column < NUMBER_OF_PIECE_COLUMNS; column++) {
			PositionPoint position = {(float) FIRST_P1PIECE_POSITION_X + (float) column * (float) (float) SPACE_BETWEEN_PIECES, (float) FIRST_P1PIECE_POSITION_Y, (float) FIRST_P1PIECE_POSITION_Z - ((float) NUMBER_OF_PIECE_ROWS - (float) row) * (float) SPACE_BETWEEN_PIECES};
			printf("x= %f, y= %f, z= %f\n", position.x, position.y, position.z);
			boardPiecesInitialPositions.insert(pair<unsigned int, PositionPoint>(p1ID, position));
			boardPieces.find(p1ID)->second->position[0] = position.x;
			boardPieces.find(p1ID)->second->position[1] = position.y;
			boardPieces.find(p1ID)->second->position[2] = position.z;
            boardPieces.find(p1ID)->second->player = 'x';
			p1ID++;

			PositionPoint restPosition;
			restPosition.x = (float) FIRST_P1REST_POS_X - (float) row * (float) SPACE_BETWEEN_PIECES;
			restPosition.y = (float) FIRST_P1REST_POS_Y;
			restPosition.z = (float) FIRST_P1REST_POS_Z + (float) column * (float) SPACE_BETWEEN_PIECES;
			p1RestPositions.push(restPosition);
		}
	}
	//On the last row, we want two pieces "centered" in front of the others
	PositionPoint p1position1 = {(float) FIRST_P1PIECE_POSITION_X + (float) SPACE_BETWEEN_PIECES, (float) FIRST_P1PIECE_POSITION_Y, (float) FIRST_P1PIECE_POSITION_Z - ((float) NUMBER_OF_PIECE_ROWS - 1) * (float) SPACE_BETWEEN_PIECES};
	boardPiecesInitialPositions.insert(pair<unsigned int, PositionPoint>(p1ID, p1position1));
	boardPieces.find(p1ID)->second->position[0] = p1position1.x;
	boardPieces.find(p1ID)->second->position[1] = p1position1.y;
	boardPieces.find(p1ID)->second->position[2] = p1position1.z;
    boardPieces.find(p1ID)->second->player = 'x';
	p1ID++;

	PositionPoint p1RestPosition1;
	p1RestPosition1.x = (float) FIRST_P1REST_POS_X - (float) row * (float) SPACE_BETWEEN_PIECES;
	p1RestPosition1.y = (float) FIRST_P1REST_POS_Y;
	p1RestPosition1.z = (float) FIRST_P1REST_POS_Z + (float) SPACE_BETWEEN_PIECES;
	p1RestPositions.push(p1RestPosition1);
    	
	PositionPoint p1position2 = {(float) FIRST_P1PIECE_POSITION_X + 3.0 * (float) SPACE_BETWEEN_PIECES, (float) FIRST_P1PIECE_POSITION_Y, (float) FIRST_P1PIECE_POSITION_Z - ((float) NUMBER_OF_PIECE_ROWS - 1) * (float) SPACE_BETWEEN_PIECES};
	boardPiecesInitialPositions.insert(pair<unsigned int, PositionPoint>(p1ID, p1position2));
	boardPieces.find(p1ID)->second->position[0] = p1position2.x;
	boardPieces.find(p1ID)->second->position[1] = p1position2.y;
	boardPieces.find(p1ID)->second->position[2] = p1position2.z;
    boardPieces.find(p1ID)->second->player = 'x';

	PositionPoint p1RestPosition2;
	p1RestPosition2.x = (float) FIRST_P1REST_POS_X - (float) row * (float) SPACE_BETWEEN_PIECES;
	p1RestPosition2.y = (float) FIRST_P1REST_POS_Y;
	p1RestPosition2.z = (float) FIRST_P1REST_POS_Z + 3.0 * (float) SPACE_BETWEEN_PIECES;
	p1RestPositions.push(p1RestPosition2);
    
	//Load Player 2 pieces
	unsigned int p2ID = NUMBER_OF_PLAYER_PIECES + 1 + NUMBER_OF_SQUARE_COLUMNS * NUMBER_OF_SQUARE_ROWS;
	for(row = 0; row < NUMBER_OF_PIECE_ROWS - 1; row++) {
		for(column = 0; column < NUMBER_OF_PIECE_COLUMNS; column++) {
			PositionPoint position;
			position.x = (float) FIRST_P2PIECE_POSITION_X + (float) column * (float) SPACE_BETWEEN_PIECES;
			position.y = (float) FIRST_P2PIECE_POSITION_Y;
			position.z = (float) FIRST_P2PIECE_POSITION_Z + (float) row * (float) SPACE_BETWEEN_PIECES;
			boardPiecesInitialPositions.insert(pair<unsigned int, PositionPoint>(p2ID, position));
			boardPieces.find(p2ID)->second->position[0] = position.x;
			boardPieces.find(p2ID)->second->position[1] = position.y;
			boardPieces.find(p2ID)->second->position[2] = position.z;
            boardPieces.find(p2ID)->second->player = 'o';
			p2ID++;

			PositionPoint restPosition;
			restPosition.x = (float) FIRST_P2REST_POS_X + (float) row * (float) SPACE_BETWEEN_PIECES;
			restPosition.y = (float) FIRST_P2REST_POS_Y;
			restPosition.z = (float) FIRST_P2REST_POS_Z + (float) column * (float) SPACE_BETWEEN_PIECES;
			p2RestPositions.push(restPosition);
		}
	}
	//On the last row, we want two pieces "centered" in front of the others
	PositionPoint p2position1 = {(float) FIRST_P2PIECE_POSITION_X + (float) SPACE_BETWEEN_PIECES, (float) FIRST_P2PIECE_POSITION_Y, (float) FIRST_P2PIECE_POSITION_Z + (float) row * (float) SPACE_BETWEEN_PIECES};
	boardPiecesInitialPositions.insert(pair<unsigned int, PositionPoint>(p2ID, p2position1));
	boardPieces.find(p2ID)->second->position[0] = p2position1.x;
	boardPieces.find(p2ID)->second->position[1] = p2position1.y;
	boardPieces.find(p2ID)->second->position[2] = p2position1.z;
    boardPieces.find(p2ID)->second->player = 'o';
	p2ID++;

	PositionPoint p2RestPosition1;
	p2RestPosition1.x = (float) FIRST_P2REST_POS_X + (float) row * (float) SPACE_BETWEEN_PIECES;
	p2RestPosition1.y = (float) FIRST_P2REST_POS_Y;
	p2RestPosition1.z = (float) FIRST_P2REST_POS_Z + (float) SPACE_BETWEEN_PIECES;
	p2RestPositions.push(p2RestPosition1);
    
	PositionPoint p2position2 = {(float) FIRST_P2PIECE_POSITION_X + 3.0 * (float) SPACE_BETWEEN_PIECES, (float) FIRST_P2PIECE_POSITION_Y, (float) FIRST_P2PIECE_POSITION_Z + (float) row * (float) SPACE_BETWEEN_PIECES};
	boardPiecesInitialPositions.insert(pair<unsigned int, PositionPoint>(p2ID, p2position2));
	boardPieces.find(p2ID)->second->position[0] = p2position2.x;
	boardPieces.find(p2ID)->second->position[1] = p2position2.y;
	boardPieces.find(p2ID)->second->position[2] = p2position2.z;
    boardPieces.find(p2ID)->second->player = 'o';

	PositionPoint p2RestPosition2;
	p2RestPosition2.x = (float) FIRST_P2REST_POS_X + (float) row * (float) SPACE_BETWEEN_PIECES;
	p2RestPosition2.y = (float) FIRST_P2REST_POS_Y;
	p2RestPosition2.z = (float) FIRST_P2REST_POS_Z + 3.0 * (float) SPACE_BETWEEN_PIECES;
	p2RestPositions.push(p2RestPosition2);
    
	map<unsigned int, PositionPoint>::iterator it = boardPiecesInitialPositions.begin();
	printf("size: %lu\n", boardPiecesInitialPositions.size());
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
    
    player1Type = HUMAN;
    player2Type = MEDIUM;
    
    timeout = 10;
    turnStart = 0;
    
    try {
        GameState gameState = choko.initializeGame();
        gameStates.push(gameState);
    } catch (InvalidMove &invalid) {
        printf("Initialization error.\n");
    }
}

// This will execute a move
// If the move is an attack, the target squareID to be removed is returned!
// And if the board has a second enemy to be removed, it will trigger the SELECT_SECOND_ENEMY
// state and won't execute the move in choko! The player must select a second enemy and call
// executeMove(int, PositionPoint, int), so that the move will be executed in choko
int Game::executeMove(int pieceID, PositionPoint destination) {
    BoardPiece * boardPiece = getBoardPiece(pieceID);
    int moveFrom = boardPiece->squareID;
    int moveTo = getPickingSquareID(destination);
    int targetToRemove = 0;
    
    if (moveFrom == 0) {
        // This is a drop
        try {
            GameState newState = choko.execute(getGameState(), to_string(moveTo));
            gameStates.push(newState);
            turnStart = time;
			boardPiece->onBoard = true;
			MovementHistoryElement lastMove(DROP, boardPiece->id, 0, 0);
			movementHistory.push(lastMove);
        } catch (InvalidMove &invalid) {
            printf("Invalid move!!\n");
        }
	} else {
        // This is a move or attack
        try {
            stringstream move;
            move << moveFrom;
            bool isAttack = true;
            PieceMoves available = choko.getPieceMoves(getGameState(), moveFrom);
            for (int i = 0; i < available.moves.size(); i++) {
                if (available.moves.at(i) == moveTo) {
                    move << '-' << moveTo;
                    isAttack = false;

					MovementHistoryElement lastMove(MOVE, boardPiece->id, 0, 0);
					movementHistory.push(lastMove);
                    break;
                }
            }
            if (isAttack) {
                for (int i = 0; i < available.attacks.size(); i++) {
                    if (available.attacks.at(i) == moveTo) {
                        move << '-' << moveTo << '-';
						targetToRemove = available.targets.at(i);
                        if ( getPiecesOnBoard(boardPiece->getOpponent()) > 1){
                            // there are more enemies in the board
                            setSelectState(SELECT_SECOND_ENEMY);
							firstAttackingOrigin = getBoardPiecePosition(pieceID);
                            firstAttackingDestination = destination;
							firstCapturedPieceID = getPieceOnSquare(targetToRemove);
							setBoardPiecePosition(pieceID, destination);
                            return targetToRemove;
                        }
                        // there aren't more enemies in the board
                        move << '0';
						MovementHistoryElement lastMove(ATTACK, boardPiece->id, getPieceOnSquare(targetToRemove), 0);
						movementHistory.push(lastMove);
                    }
                }
            }
            GameState newState = choko.execute(getGameState(), move.str());
            gameStates.push(newState);
            turnStart = time;
        } catch (InvalidMove &invalid) {
            printf("Invalid move!!\n");
        }
        
    }
	setBoardPiecePosition(pieceID, destination);
    return targetToRemove;
}

int Game::executeMove(PositionPoint firstAttackingOrigin, PositionPoint firstAttackingDestination, int secondEnemyPieceID) {
    int moveFrom = getPickingSquareID(firstAttackingOrigin);
    int moveTo = getPickingSquareID(firstAttackingDestination);
    
    BoardPiece * secondEnemyPiece = getBoardPiece(secondEnemyPieceID);
    int removeFrom = secondEnemyPiece->squareID;
    
    try {
        stringstream move;
        move << moveFrom << '-' << moveTo << '-' << removeFrom;
        GameState newState = choko.execute(getGameState(), move.str());
        gameStates.push(newState);
		MovementHistoryElement lastMove(ATTACK, getPieceWithPosition(firstAttackingOrigin), firstCapturedPieceID, secondEnemyPieceID);
		movementHistory.push(lastMove);
        turnStart = time;
    } catch (InvalidMove &invalid) {
        printf("Invalid move!!\n");
    }
	setSelectState(SELECT_ANY);
    return 0;
}

void Game::addPiece(BoardPiece* piece) {
	boardPieces.insert(pair<unsigned int, BoardPiece*>(piece->id, piece));
}

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

string Game::getPieceIDStr(unsigned int id) {
	return to_string(id - NUMBER_OF_SQUARE_COLUMNS * NUMBER_OF_SQUARE_ROWS);
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
		if(it->second->position[0] == position.x && it->second->position[1] == position.y && it->second->position[2] == position.z) {
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
	if(isBoardPiece(squareID))
		return false;
    
	PositionPoint p;
	p = getPickingSquarePosition(squareID);
    
	if(getPieceWithPosition(p) != -1)
		return false;
    
    BoardPiece * selectedPiece = getBoardPiece(selectedPieceID);
    if (selectedPiece->squareID == 0)
        return true; // the selected piece is outside the board, it can go in
    
    PieceMoves available = choko.getPieceMoves(getGameState(), selectedPiece->squareID);
    for (int i = 0; i < available.moves.size(); i++) {
        if (available.moves.at(i) == squareID)
            return true;
    }
    for (int i = 0; i < available.attacks.size(); i++) {
        if (available.attacks.at(i) == squareID)
            return true;
    }
    
	return false;
}

BoardPiece * Game::getBoardPiece(unsigned int pieceID) {
	map<unsigned int, BoardPiece*>::iterator it = boardPieces.find(pieceID);
	if(it != boardPieces.end()) {
		return it->second;
	}
	return NULL;
}

PositionPoint Game::getBoardPiecePosition(unsigned int pieceID) {
	PositionPoint position = {0.0, 0.0, 0.0};
    
	map<unsigned int, BoardPiece*>::iterator it = boardPieces.find(pieceID);
	if(it != boardPieces.end()) {
		position.x = (it->second->position[0]);
		position.y = (it->second->position[1]);
		position.z = (it->second->position[2]);
	}
    
	return position;
}

PositionPoint Game::getPickingSquarePosition(unsigned int squareID) {
	PositionPoint position = {0.0, 0.0, 0.0};
    
	map<unsigned int, PositionPoint>::iterator it = pickingSquaresPositions.find(squareID);
	if(it != pickingSquaresPositions.end()) {
		position.x = (it->second.x);
		position.y = (it->second.y);
		position.z = (it->second.z);
	}
    
	return position;
}

int Game::getPickingSquareID(PositionPoint position){
    map<unsigned int, PositionPoint>::iterator it = pickingSquaresPositions.begin();
    for(; it != pickingSquaresPositions.end(); it++) {
        if(it->second.x == position.x &&
           it->second.y == position.y &&
           it->second.z == position.z) {
            return it->first;
        }
    }
    
    return -1;
}

int Game::getPieceOnSquare(int squareID) {
    if (squareID > NUMBER_OF_SQUARES && squareID <= NUMBER_OF_SQUARES + NUMBER_OF_PLAYER_PIECES * 2) {
        // we already clicked on the piece we want
        return squareID;
    }
    PositionPoint piecePosition = getPickingSquarePosition(squareID);
    return getPieceWithPosition(piecePosition);
}

void Game::setBoardPiecePosition(unsigned int pieceID, PositionPoint position) {
	map<unsigned int, BoardPiece*>::iterator it = boardPieces.find(pieceID);
    BoardPiece * piece = NULL;
	if(it != boardPieces.end()) {
        piece = it->second;
		float* previousPosition = new float[3];
		previousPosition[0] = piece->position[0];
		previousPosition[1] = piece->position[1];
		previousPosition[2] = piece->position[2];
		piece->previousPositions.push(previousPosition);
		
		piece->position[0] = position.x;
		piece->position[1] = position.y;
		piece->position[2] = position.z;
	}
    
    if (piece == NULL) return;
    
	int squareID = getPickingSquareID(position);
    setBoardPieceSquare(pieceID, squareID);
}

GameState Game::getGameState() {
    return gameStates.top();
}

void Game::setBoardPieceSquare(unsigned int pieceID, int squareID) {
    BoardPiece * piece = getBoardPiece(pieceID);
    if (piece != NULL) {
		piece->previousSquares.push(piece->squareID);
		if(squareID != -1)
			piece->squareID = squareID;
	}
}

char Game::getCurrentPlayer() {
	return getGameState().currentPlayer;
}

bool Game::isOwnPiece(int pieceID) {
	BoardPiece* selectedPiece = getBoardPiece(pieceID);
	if(selectedPiece == NULL)
		return false;

	if(selectedPiece->playable && (selectedPiece->player == getCurrentPlayer()))
		return true;
	else
		return false;
}

int Game::getPiecesOnBoard(char player) {
    int counter = 0;
    map<unsigned int, BoardPiece*>::iterator it = boardPieces.begin();
    BoardPiece * piece = NULL;
	while(it != boardPieces.end()) {
        piece = it->second;
		if (piece->onBoard && piece->player == player)
            counter++;
        it++;
	}
    return counter;
}

PositionPoint Game::getNextP1RestPosition() {
	PositionPoint position = p1RestPositions.top();
	return position;
}

PositionPoint Game::getNextP2RestPosition() {
	PositionPoint position = p2RestPositions.top();
	return position;
}

PositionPoint Game::getPieceRestPosition(BoardPiece* piece) {
	if(piece->player == 'x')
		return getNextP1RestPosition();
	else
		return getNextP2RestPosition();
}

PositionPoint Game::getSecondP1RestPosition() {
	PositionPoint firstPosition = p1RestPositions.top();
	p1RestPositions.pop();
	PositionPoint secondPosition = p1RestPositions.top();
	p1RestPositions.push(firstPosition);
	return secondPosition;
}

PositionPoint Game::getSecondP2RestPosition() {
	PositionPoint firstPosition = p2RestPositions.top();
	p2RestPositions.pop();
	PositionPoint secondPosition = p2RestPositions.top();
	p2RestPositions.push(firstPosition);
	return secondPosition;
}

PositionPoint Game::getSecondPieceRestPosition(BoardPiece* piece) {
	if(piece->player == 'x')
		return getSecondP1RestPosition();
	else
		return getSecondP2RestPosition();
}

void Game::popPieceRestPosition(BoardPiece* piece) {
	if(piece->player == 'x')
		p1RestPositions.pop();
	else
		p2RestPositions.pop();
}

bool Game::canCapture(int pieceID) {
    if (pieceID < 0)
        return false;
    
    BoardPiece * piece = getBoardPiece(pieceID);
    if (piece->onBoard && piece->player != getGameState().currentPlayer)
        return true;
    else
        return false;
}

void Game::capture(int secondPieceID) {
    executeMove(firstAttackingOrigin, firstAttackingDestination, secondPieceID);
}

bool Game::currentPlayerIsAI() {
	bool player1IsComputer = false;
	if (player1Type == EASY) {
		player1IsComputer = true;
	}

	if (player1Type == MEDIUM) {
		player1IsComputer = true;
	}

	if (player1Type == HARD) {
		player1IsComputer = true;
	}

	bool player2IsComputer = false;

	if (player2Type == EASY) {
		player2IsComputer = true;
	}

	if (player2Type == MEDIUM) {
		player2IsComputer = true;
	}

	if (player2Type == HARD) {
		player2IsComputer = true;
	}

	return( (player1IsComputer && getGameState().currentPlayer == 'x') || (player2IsComputer && getGameState().currentPlayer == 'o') );
}

bool Game::hasGameEnded() {
	return(getGameState().gameOver);
}

int Game::getWinner() {
	if(getGameState().winner == 'x')
		return 1;
	else if(getGameState().winner == 'o')
		return 2;
	else
		return -1;
}

void Game::updateAI() {
    if (hasGameEnded())
        return;
    
    bool player1IsComputer = false;
    if (player1Type == EASY) {
        player1IsComputer = true;
    }
    
    if (player1Type == MEDIUM) {
        player1IsComputer = true;
    }
    
    if (player1Type == HARD) {
        player1IsComputer = true;
    }
    
    if (player1IsComputer && getGameState().currentPlayer == 'x') {
        calculateMove(player1Type);
		return;
    }
    
    
    bool player2IsComputer = false;
    
    if (player2Type == EASY) {
        player2IsComputer = true;
    }
    
    if (player2Type == MEDIUM) {
        player2IsComputer = true;
    }
    
    if (player2Type == HARD) {
        player2IsComputer = true;
    }

    if (player2IsComputer && getGameState().currentPlayer == 'o') {
        calculateMove(player2Type);
		return;
    }
    
}

int Game::calculateMove(int playerType) {
    try {
        GameState newState = choko.calculate(getGameState(), playerTypes[playerType]);
        gameStates.push(newState);
        turnStart = time;
    } catch (InvalidMove &invalid) {
        printf("AI error!\n");
    }
    return 0;
}

void Game::processAIMovedPieces(Move move) {
	if(move.moveType == DROP) {
		BoardPiece* piece = getUnusedPiece(move.player);
		PositionPoint destination = getPickingSquarePosition(move.toSquare);
		setBoardPiecePosition(piece->id, destination);
		piece->onBoard = true;

		MovementHistoryElement lastMove(DROP, piece->id, 0, 0);
		movementHistory.push(lastMove);
		return;
	}

	if(move.moveType == MOVE) {
		int pieceID = getPieceOnSquare(move.fromSquare);
		PositionPoint destination = getPickingSquarePosition(move.toSquare);
		setBoardPiecePosition(pieceID, destination);

		MovementHistoryElement lastMove(MOVE, pieceID, 0, 0);
		movementHistory.push(lastMove);
		return;
	}

	if(move.moveType == ATTACK) {
		int attackerSquare = move.fromSquare;
		int firstCapturedSquare = move.firstAttackSquare;
		int secondCapturedSquare = move.secondAttackSquare;

		int attackerPieceID = getPieceOnSquare(attackerSquare);
		int firstCapturedID = getPieceOnSquare(firstCapturedSquare);
		int secondCapturedID = 0;

		BoardPiece* attackerPiece = getBoardPiece(attackerPieceID);

		PositionPoint attackerPieceDestination = getPickingSquarePosition(move.toSquare);
		setBoardPiecePosition(attackerPiece->id, attackerPieceDestination);

		BoardPiece* firstCapturedPiece = getBoardPiece(firstCapturedID);

		PositionPoint firstCapturedPieceDestination = getPieceRestPosition(firstCapturedPiece);

		popPieceRestPosition(firstCapturedPiece);
		setBoardPiecePosition(firstCapturedPiece->id, firstCapturedPieceDestination);
		firstCapturedPiece->onBoard = false;
		firstCapturedPiece->playable = false;

		//Don't forget there may not be a second captured piece (if there's no second piece to capture)
		if(secondCapturedSquare != 0) {
			secondCapturedID = getPieceOnSquare(secondCapturedSquare);
			BoardPiece* secondCapturedPiece = getBoardPiece(secondCapturedID);
			
			PositionPoint secondCapturedPieceDestination = getPieceRestPosition(secondCapturedPiece);
			popPieceRestPosition(secondCapturedPiece);
			setBoardPiecePosition(secondCapturedPiece->id, secondCapturedPieceDestination);
			secondCapturedPiece->onBoard = false;
			secondCapturedPiece->playable = false;
		}

		MovementHistoryElement lastMove(ATTACK, attackerPieceID, firstCapturedID, secondCapturedID);
		movementHistory.push(lastMove);

		return;
	}
}

BoardPiece* Game::getUnusedPiece(char player) {
	map<unsigned int, BoardPiece*>::iterator it;
	for(it = boardPieces.begin(); it != boardPieces.end(); it++) {
		if(it->second->player == player && !it->second->onBoard && it->second->playable) {
			return it->second;
		}
	}

	return NULL;
}

void Game::undoLastMove() {
	gameStates.pop();
	MovementHistoryElement lastMove = getLastMove();
	if(lastMove.moveType == 0)
		return;
	movementHistory.pop();

	if(lastMove.moveType == DROP) {
		BoardPiece* piece = getBoardPiece(lastMove.modifiedPieces.at(0));
		piece->onBoard = false;
		piece->undoMovement();
		return;
	}

	if(lastMove.moveType == MOVE) {
		BoardPiece* piece = getBoardPiece(lastMove.modifiedPieces.at(0));
		piece->undoMovement();
		return;
	}

	if(lastMove.moveType == ATTACK) {
		BoardPiece* attackerPiece = getBoardPiece(lastMove.modifiedPieces.at(0));
		attackerPiece->undoMovement();

		BoardPiece* firstCapturedPiece = getBoardPiece(lastMove.modifiedPieces.at(1));
		firstCapturedPiece->undoMovement();
		firstCapturedPiece->onBoard = true;
		firstCapturedPiece->playable = true;
		//Restore rest position
		
		if(lastMove.modifiedPieces.at(2) != 0) {
			BoardPiece* secondCapturedPiece = getBoardPiece(lastMove.modifiedPieces.at(1));
			secondCapturedPiece->undoMovement();
			secondCapturedPiece->onBoard = true;
			secondCapturedPiece->playable = true;
			//Restore rest position
		}
	}
}

MovementHistoryElement Game::getLastMove() {
	if(movementHistory.empty())
		return MovementHistoryElement(0,0,0,0);

	MovementHistoryElement lastMove = movementHistory.top();
	return lastMove;
}

void Game::update(unsigned long t) {
    time = t;
    turnTimeLeft = (turnStart + timeout * 1000.0 - time) / 1000.0;
    if (turnTimeLeft <= 0.25) {
        skipTurn();
        turnStart = time;
    }
}

void Game::skipTurn() {
    GameState currentState = getGameState();
    GameState newState(currentState);
    
    newState.currentPlayerUnusedPieces = currentState.enemyPlayerUnusedPieces;
    newState.enemyPlayerUnusedPieces = currentState.currentPlayerUnusedPieces;

    if (currentState.currentPlayer == 'x') {
        newState.currentPlayer = 'o';
        newState.player1UnusedPieces = newState.enemyPlayerUnusedPieces;
        newState.player2UnusedPieces = newState.currentPlayerUnusedPieces;
    } else {
        newState.currentPlayer = 'x';
        newState.player1UnusedPieces = newState.currentPlayerUnusedPieces;
        newState.player2UnusedPieces = newState.enemyPlayerUnusedPieces;
    }
    
    if (currentState.dropInitiative == 'x') {
        newState.dropInitiative = 'o';
    } else {
        newState.dropInitiative = 'x';
    }
    
    newState.move = "0";
    newState.removedPieces.clear();
    newState.parsedMove = Move();
    
    gameStates.push(newState);
}

Game::~Game() {
	map<unsigned int, BoardPiece*>::iterator pieceIt = boardPieces.begin();
	for(; pieceIt != boardPieces.end(); pieceIt++) {
		delete pieceIt->second;
	}
}
