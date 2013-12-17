#include "Game.h"

BoardPiece::BoardPiece(unsigned int id): id(id), onBoard(false), playable(true), toggled(false), squareID(0) {}

//PickingSquare::PickingSquare(unsigned int id): id(id), pieceID(0), hasPiece(false) {}

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
			boardPiece->onBoard = true;
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
							attackingPiece = pieceID;
                            firstAttackingDestination = destination;
                            return targetToRemove;
                        }
                        // there aren't more enemies in the board
                        move << '0';
                    }
                }
            }
            GameState newState = choko.execute(getGameState(), move.str());
            gameStates.push(newState);
        } catch (InvalidMove &invalid) {
            printf("Invalid move!!\n");
        }
        
    }
    
    setBoardPiecePosition(pieceID, destination);
    
    return targetToRemove;
}

int Game::executeMove(int pieceID, PositionPoint destination, int secondEnemyPieceID) {
    BoardPiece * boardPiece = getBoardPiece(pieceID);
    int moveFrom = boardPiece->squareID;
    int moveTo = getPickingSquareID(destination);
    
    BoardPiece * secondEnemyPiece = getBoardPiece(secondEnemyPieceID);
    int removeFrom = secondEnemyPiece->squareID;
    
    try {
        stringstream move;
        move << moveFrom << '-' << moveTo << '-' << removeFrom;
        GameState newState = choko.execute(getGameState(), move.str());
        gameStates.push(newState);
    } catch (InvalidMove &invalid) {
        printf("Invalid move!!\n");
    }
    return 0;
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
		piece->position[0] = position.x;
		piece->position[1] = position.y;
		piece->position[2] = position.z;
	}
    
    if (piece == NULL) return;
    
    int squareID = getPickingSquareID(position);
    if (squareID != -1)
        setBoardPieceSquare(pieceID, squareID);
}

GameState Game::getGameState() {
    return gameStates.top();
}

void Game::setBoardPieceSquare(unsigned int pieceID, unsigned int squareID) {
    BoardPiece * piece = getBoardPiece(pieceID);
    if (piece != NULL)
        piece->squareID = squareID;
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
	p1RestPositions.pop();
	return position;
}

PositionPoint Game::getNextP2RestPosition() {
	PositionPoint position = p2RestPositions.top();
	p2RestPositions.pop();
	return position;
}

PositionPoint Game::getPieceRestPosition(BoardPiece* piece) {
	if(piece->player == 'x')
		return getNextP1RestPosition();
	else
		return getNextP2RestPosition();
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
    executeMove(attackingPiece, firstAttackingDestination, secondPieceID);
}