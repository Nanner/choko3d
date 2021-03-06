#include "RendererInterface.h"


RendererInterface::RendererInterface(vector<string> sceneNames): sceneNames(sceneNames) {}

void RendererInterface::initGUI() {
	this->sceneGraph = ((DemoScene*) scene)->getSceneGraph();
	RootVertex* rootVertex = sceneGraph->getRootVertex();
    vector<RootVertex *> allRootVertexes = sceneGraph->getRootVertexes();
    
    glutReshapeWindow(1300,700);
	glutPositionWindow((glutGet(GLUT_SCREEN_WIDTH)-1300)/2,
		(glutGet(GLUT_SCREEN_HEIGHT)-700)/2);
    
    mainWindow = glutGetWindow();

	int lastID = 0;
    
    GLUI_Panel* gameInfoPanel = addPanel((char*)"Game Information");
	GLUI_Listbox* currentPlayer = addListboxToPanel(gameInfoPanel, (char*)"Current Player: ", &sceneGraph->getGame()->currentPlayer, lastID);
	lastID++;
	currentPlayer->add_item(0, (char*)"Player 1 (Black)");
	currentPlayer->add_item(1, (char*)"Player 2 (White) ");
    currentPlayer->set_int_val(0);
    currentPlayer->disable();
    
    GLUI_Listbox* dropInitiative = addListboxToPanel(gameInfoPanel, (char*)"Drop initiative: ", &sceneGraph->getGame()->currentDropInitiative, lastID);
	lastID++;
	dropInitiative->add_item(0, (char*)"Player 1 (Black)");
	dropInitiative->add_item(1, (char*)"Player 2 (White)  ");
    dropInitiative->set_int_val(0);
    dropInitiative->disable();
    
    addStaticTextToPanel(gameInfoPanel, (char*)"Only the player with the drop");
    addStaticTextToPanel(gameInfoPanel, (char*)"initiative can move or attack, giving");
    addStaticTextToPanel(gameInfoPanel, (char*)"the initiative to the other player." );

	addColumn();
    
	GLUI_Panel* playersPanel = addPanel((char*)"Player Types");
    
    GLUI_Listbox* player1List = addListboxToPanel(playersPanel, (char*)"Player 1 (Black) ", &sceneGraph->getGame()->player1Type, lastID);
	lastID++;
	player1List->add_item(0, (char*)"Human");
	player1List->add_item(1, (char*)"Computer Easy");
	player1List->add_item(2, (char*)"Computer Medium");
    player1List->add_item(3, (char*)"Computer Hard");
    player1List->set_int_val(sceneGraph->getGame()->player1Type);
    
    GLUI_Listbox* player2List = addListboxToPanel(playersPanel, (char*)"Player 2 (White)  ", &sceneGraph->getGame()->player2Type, lastID);
	lastID++;
	player2List->add_item(0, (char*)"Human");
	player2List->add_item(1, (char*)"Computer Easy");
	player2List->add_item(2, (char*)"Computer Medium");
    player2List->add_item(3, (char*)"Computer Hard");
    player2List->set_int_val(sceneGraph->getGame()->player2Type);
    
    undoButton = addButton((char*)"Undo move", lastID);
	undoButtonID = lastID;
	lastID++;
    
    GLUI_Spinner* animationDurationSpinner = this->glui_window->add_spinner((char*)"Move duration", GLUI_SPINNER_FLOAT, &SceneVertex::moveAnimationDuration, lastID);
    lastID++;
    animationDurationSpinner->set_float_limits(0.001, 100.0);
    
    cameraRotationCheckbox = addCheckbox((char*)"Auto camera", &sceneGraph->getGame()->cameraController->enabled, lastID);
    cameraRotationID = lastID;
    lastID++;

    addColumn();
    
    GLUI_Panel * scorePanel = addPanel((char*)"Scores");
    GLUI_Spinner* player1ScoreSpinner = this->glui_window->add_spinner_to_panel(scorePanel, (char*)"Player 1 ", GLUI_SPINNER_INT, &sceneGraph->getGame()->player1Score, lastID);
    player1ScoreSpinner->disable();
    lastID++;
    
    GLUI_Spinner* player2ScoreSpinner = this->glui_window->add_spinner_to_panel(scorePanel, (char*)"Player 2 ", GLUI_SPINNER_INT, &sceneGraph->getGame()->player2Score, lastID);
    player2ScoreSpinner->disable();
    lastID++;
    
    GLUI_Panel * timePanel = addPanel((char*)"Turn Clock");
    

    GLUI_Spinner* timeSpinner = this->glui_window->add_spinner_to_panel(timePanel, (char*)"Time left", GLUI_SPINNER_FLOAT, &sceneGraph->getGame()->turnTimeLeft, lastID);
    timeSpinner->disable();
    lastID++;
    
    GLUI_Spinner* timePerTurnSpinner = this->glui_window->add_spinner_to_panel(timePanel, (char*)"Time per turn", GLUI_SPINNER_FLOAT, &sceneGraph->getGame()->timeout, lastID);
    timePerTurnSpinner->set_float_limits(0.1, 100000.0);
    lastID++;

    //  Create GLUI window for game over
    gameOverWindow = GLUI_Master.create_glui ("Game Over");
    
    gameOverPanel = gameOverWindow->add_panel((char*)"Game Over");
        
    winnerText = addStaticTextToPanel(gameOverPanel, (char*)"Winner is ...");
    addButtonToPanel(gameOverPanel, (char*)"Restart Game", lastID);
	gameOverGameRestartButtonID = lastID;
	lastID++;
    addButtonToPanel(gameOverPanel, (char*)"Replay Game", lastID);
	gameOverFilmButtonID = lastID;
	lastID++;
    gameOverWindow->hide();
    gameOverWindowVisible = false;
    
    //  Create GLUI window for no moves possible
    noMovesWindow = GLUI_Master.create_glui("No possible moves");
    
    noMovesPanel = noMovesWindow->add_panel((char*)"No possible moves");
    
    addStaticTextToPanel(noMovesPanel, (char*)"There aren't any possible moves for this player, his turn will be skipped.");
    addButtonToPanel(noMovesPanel, (char*)"Ok", lastID);
	turnSkipButtonID = lastID;
	lastID++;
    noMovesWindow->hide();
    noMovesWindowVisible = false;

	//  Create GLUI window
	filmOverWindow = GLUI_Master.create_glui ("Film Ended");

	filmOverPanel = filmOverWindow->add_panel((char*)"Film Ended");

	filmOverText = addStaticTextToPanel(filmOverPanel, (char*)"Film is over!");
	addButtonToPanel(filmOverPanel, (char*)"Restart Game", lastID);
	filmOverGameRestartButtonID = lastID;
	lastID++;
	addButtonToPanel(filmOverPanel, (char*)"Replay Game", lastID);
	filmOverFilmButtonID = lastID;
	filmOverWindow->hide();
	filmOverWindowVisible = false;
	lastID++;
    
    addColumn();
    
    GLUI_Panel* scenePanel = addPanel((char*)"Choose Scene");
    
    sceneList = addListboxToPanel(scenePanel, (char*)"Choose Scene ", &sceneGraph->currentScene, lastID);
    sceneSwitchID = lastID;
	lastID++;
    for (int i = 0; i < sceneNames.size(); i++) {
        sceneList->add_item(i, sceneNames.at(i).c_str());
    }
    sceneList->set_int_val(sceneGraph->currentScene);
    
    GLUI_Panel* drawmodesPanel = addPanel((char*)"Draw modes");
	GLUI_Listbox* drawmodeList = addListboxToPanel(drawmodesPanel, (char*)"Draw mode: ", &((DemoScene*)scene)->activeDrawMode, lastID);
	lastID++;
	drawmodeList->add_item(0, (char*)"Fill");
	drawmodeList->add_item(1, (char*)"Line");
	drawmodeList->add_item(2, (char*)"Point");
	if(rootVertex->globals.drawmode == "fill")
		drawmodeList->set_int_val(0);
	else if(rootVertex->globals.drawmode == "line")
		drawmodeList->set_int_val(1);
	else if(rootVertex->globals.drawmode == "point")
		drawmodeList->set_int_val(2);
    
    popupWindowOn = false;
}

void RendererInterface::processGUI(GLUI_Control *ctrl) {
	RootVertex* rootVertex = sceneGraph->getRootVertex();

	map<int, string>::iterator lightToToggle = lightMap.find(ctrl->user_id);
	if(lightToToggle != lightMap.end()) {
		if (!rootVertex->lightOnControls.find(lightToToggle->second)->second){
			rootVertex->lights.find(lightToToggle->second)->second->disable();
		} else {
			rootVertex->lights.find(lightToToggle->second)->second->enable();
		}
		return;
	}

	if(ctrl->user_id == gameOverGameRestartButtonID) {
		((DemoScene*) scene)->restartGameOnNextUpdate();
        gameOverWindow->hide();
        gameOverWindowVisible = false;
        popupWindowOn = false;
	}

	if(ctrl->user_id == gameOverFilmButtonID) {
		((DemoScene*) scene)->startFilmMode();
		gameOverWindow->hide();
		gameOverWindowVisible = false;
        popupWindowOn = false;
	}

	if(ctrl->user_id == filmOverGameRestartButtonID) {
		((DemoScene*) scene)->restartGameOnNextUpdate();
		filmOverWindow->hide();
		filmOverWindowVisible = false;
        popupWindowOn = false;
	}

	if(ctrl->user_id == filmOverFilmButtonID) {
		((DemoScene*) scene)->startFilmMode();
		filmOverWindow->hide();
		filmOverWindowVisible = false;
        popupWindowOn = false;
	}

	if(ctrl->user_id == undoButtonID && !popupWindowOn) {
		sceneGraph->undoLastMove();
	}
    
    if(ctrl->user_id == turnSkipButtonID) {
        sceneGraph->getGame()->skipTurn();
        noMovesWindow->hide();
        noMovesWindowVisible = false;
        popupWindowOn = false;
    }
    
	if(ctrl->user_id == cameraRotationID) {
		if(!autoCameraOn) {
			char player = sceneGraph->getGame()->currentPlayer;
			CameraController * cam = sceneGraph->getGame()->cameraController;
			cam->changeCamera();
			if (player == PLAYER1) {
				cam->setAngle(PLAYER1_ANGLE);
			} else {
				cam->setAngle(PLAYER2_ANGLE);
			}
			autoCameraOn = true;
		}
		else {
			autoCameraOn = false;
			CameraController * cam = sceneGraph->getGame()->cameraController;
			cam->changeCamera();
			cam->resetFreeCam();
		}
	}
    
    if(ctrl->user_id == sceneSwitchID) {
		deselectCurrentPiece();
		((DemoScene*) scene)->restartGameOnNextUpdate();
        sceneGraph->switchScene(sceneGraph->currentScene);
    }
}

void RendererInterface::processMouse(int button, int state, int x, int y) {
	Game* game = sceneGraph->getGame();
    
	if(game->cameraController->getEnabledCameraType() != AUTO_CAMERA)
		CGFinterface::processMouse(button,state, x, y);

	if(game->currentPlayerIsAI() || game->cameraController->isChangingFocus || game->onSkippedTurn) {
		return;
	}

	// do picking on mouse press (GLUT_DOWN)
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		performPicking(x,y);

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		deselectCurrentPiece();
	}
}

void RendererInterface::performPicking(int x, int y) {
	// Sets the buffer to be used for selection and activate selection mode
	glSelectBuffer (BUFSIZE, selectBuf);
	glRenderMode(GL_SELECT);
	((DemoScene*) scene)->isSelectMode = true;

	// Initialize the picking name stack
	glInitNames();

	// The process of picking manipulates the projection matrix
	// so we will be activating, saving and manipulating it
	glMatrixMode(GL_PROJECTION);

	//store current projmatrix to restore easily in the end with a pop
	glPushMatrix ();

	//get the actual projection matrix values on an array of our own to multiply with pick matrix later
	GLfloat projmat[16];
	glGetFloatv(GL_PROJECTION_MATRIX,projmat);

	// reset projection matrix
	glLoadIdentity();

	// get current viewport and use it as reference for 
	// setting a small picking window of 5x5 pixels around mouse coordinates for picking
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// this is multiplied in the projection matrix
	gluPickMatrix ((GLdouble) x, (GLdouble) (CGFapplication::height - y), 5.0, 5.0, viewport);

	// multiply the projection matrix stored in our array to ensure same conditions as in normal render
	glMultMatrixf(projmat);

	// force scene drawing under this mode
	// only the names of objects that fall in the 5x5 window will actually be stored in the buffer
	scene->display();

	// restore original projection matrix
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();

	glFlush();

	// revert to render mode, get the picking results and process them
	GLint hits;
	hits = glRenderMode(GL_RENDER);
	((DemoScene*) scene)->isSelectMode = false;
	processHits(hits, selectBuf);
}

void RendererInterface::processHits (GLint hits, GLuint buffer[]) {
	GLuint *ptr = buffer;
	GLuint mindepth = 0xFFFFFFFF;
	GLuint *selected=NULL;
	GLuint nselected;

	// iterate over the list of hits, and choosing the one closer to the viewer (lower depth)
	for (int i=0;i<hits;i++) {
		int num = *ptr; ptr++;
		GLuint z1 = *ptr; ptr++;
		ptr++;
		if (z1 < mindepth && num>0) {
			mindepth = z1;
			selected = ptr;
			nselected=num;
		}
		for (int j=0; j < num; j++) 
			ptr++;
	}

	// if there were hits, the one selected is in "selected", and it consist of nselected "names" (integer ID's)
	if (selected!=NULL)
	{

		Game* game = sceneGraph->getGame();
		if(game->getSelectState() == SELECT_ANY) {
			if(game->isBoardPiece(selected[0]) && game->isOwnPiece(selected[0])) {
				game->selectedPieceID = selected[0];
				game->setSelectState(SELECT_TO_SQUARE);
                game->updateHighlightedSquarePositions();
			}
		}
		else if(game->getSelectState() == SELECT_TO_SQUARE) {
			unsigned int selectedPosition = selected[0];
			if(game->canMoveTo(selectedPosition)) {
				game->setSelectState(SELECT_ANY);

				PositionPoint origin = game->getPiecePosition(game->selectedPieceID);
				PositionPoint destination = game->getSquarePosition(selectedPosition);
				sceneGraph->movePiece(game->selectedPieceID, origin, destination);
				int squareToRemove = game->executeMove(game->selectedPieceID, destination);
				if(squareToRemove != 0) {
					int removePieceID = game->getPieceOnSquare(squareToRemove);
					BoardPiece* piece = game->getPiece(removePieceID);
					piece->onBoard = false;
					piece->playable = false;
					string removePieceIDStr = game->getPieceIDStr(piece->id);
					PositionPoint removePieceOrigin = game->getSquarePosition(piece->squareID);
					PositionPoint  restPoint = game->getPieceRestPosition(piece);
					game->popPieceRestPosition(piece);
					sceneGraph->movePiece(removePieceID, removePieceOrigin, restPoint);
					game->setPiecePosition(removePieceID, restPoint);
				}
                // remove highlight from selected piece
                game->selectedPieceID = 0;
                game->updateHighlightedSquarePositions();
			}
		}
		else if(game->getSelectState() == SELECT_SECOND_ENEMY) {
			unsigned int selectedPosition = selected[0];
			int capturedPieceID = game->getPieceOnSquare(selectedPosition);
			if(game->canCapture(capturedPieceID)) {
				game->capture(capturedPieceID);
				BoardPiece* capturedPiece = game->getPiece(capturedPieceID);
				PositionPoint origin = game->getPiecePosition(capturedPieceID);
				PositionPoint restPoint = game->getPieceRestPosition(capturedPiece);
				game->popPieceRestPosition(capturedPiece);
				capturedPiece->onBoard = false;
				capturedPiece->playable = false;
				sceneGraph->movePiece(capturedPieceID, origin, restPoint);
				game->setPiecePosition(capturedPieceID, restPoint);
			}
		}
	}
	else {
        deselectCurrentPiece();
    }
}

void RendererInterface::deselectCurrentPiece() {
    Game* game = sceneGraph->getGame();
    if(game->getSelectState() != SELECT_SECOND_ENEMY) {
        game->setSelectState(SELECT_ANY);
    }
    game->selectedPieceID = 0;
    game->updateHighlightedSquarePositions();
}

void RendererInterface::updateGameOver() {
    Game* game = sceneGraph->getGame();
    if(game->hasGameEnded() && !gameOverWindowVisible) {
		string winner;
        if (game->getWinner() == 1)
            winner = "Winner is Player 1, Black!";
        else
            winner = "Winner is Player 2, White!";
        winnerText->set_text(winner.c_str());
        
        int mainWindowX = glutGet(GLUT_WINDOW_X);
        int mainWindowY = glutGet(GLUT_WINDOW_Y);
        int mainWindowWidth = glutGet(GLUT_WINDOW_WIDTH);
        int mainWindowHeight = glutGet(GLUT_WINDOW_HEIGHT);
        
        glutSetWindow(gameOverWindow->get_glut_window_id());
        
        int popupWindowWidth = 200;
        int popupWindowHeight = 100;
        
        glutReshapeWindow(popupWindowWidth, popupWindowHeight);
        
        int x = mainWindowX + mainWindowWidth / 2 - popupWindowWidth / 2;
        int y = mainWindowY + mainWindowHeight / 2 - popupWindowHeight / 2;
        
        glutPositionWindow(x, y);
        
        glutSetWindow(mainWindow);
        
        gameOverWindow->show();
        gameOverWindowVisible = true;
        popupWindowOn = true;
	}
}

void RendererInterface::updateNoMoves() {
    Game* game = sceneGraph->getGame();
    if (!game->movesPossible && !noMovesWindowVisible) {
        int mainWindowX = glutGet(GLUT_WINDOW_X);
        int mainWindowY = glutGet(GLUT_WINDOW_Y);
        int mainWindowWidth = glutGet(GLUT_WINDOW_WIDTH);
        int mainWindowHeight = glutGet(GLUT_WINDOW_HEIGHT);
        
        glutSetWindow(noMovesWindow->get_glut_window_id());
        
        int popupWindowWidth = glutGet(GLUT_WINDOW_WIDTH);
        int popupWindowHeight = glutGet(GLUT_WINDOW_WIDTH);
        
        int x = mainWindowX + mainWindowWidth / 2 - popupWindowWidth / 2;
        int y = mainWindowY + mainWindowHeight / 2 - popupWindowHeight / 2;
        
        glutPositionWindow(x, y);
        
        glutSetWindow(mainWindow);
        
        noMovesWindow->show();
        noMovesWindowVisible = true;
        popupWindowOn = true;
    }
}

void RendererInterface::updateFilmOver() {
	if( ((DemoScene*) scene)->filmMode && ((DemoScene*) scene)->filmEnded && !filmOverWindowVisible) {
		int mainWindowX = glutGet(GLUT_WINDOW_X);
		int mainWindowY = glutGet(GLUT_WINDOW_Y);
		int mainWindowWidth = glutGet(GLUT_WINDOW_WIDTH);
		int mainWindowHeight = glutGet(GLUT_WINDOW_HEIGHT);

		glutSetWindow(filmOverWindow->get_glut_window_id());

		int popupWindowWidth = 200;
		int popupWindowHeight = 100;

		glutReshapeWindow(popupWindowWidth, popupWindowHeight);

		int x = mainWindowX + mainWindowWidth / 2 - popupWindowWidth / 2;
		int y = mainWindowY + mainWindowHeight / 2 - popupWindowHeight / 2;

		glutPositionWindow(x, y);
        
        glutSetWindow(mainWindow);

		filmOverWindow->show();
		filmOverWindowVisible = true;
        popupWindowOn = true;
	}
}


