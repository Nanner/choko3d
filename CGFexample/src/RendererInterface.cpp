#include "RendererInterface.h"


RendererInterface::RendererInterface() {}

void RendererInterface::initGUI() {
	this->sceneGraph = ((DemoScene*) scene)->getSceneGraph();
	RootVertex* rootVertex = sceneGraph->getRootVertex();

	int lastID = 0;
	GLUI_Panel* lightsPanel = addPanel( (char*)"Lights", 1);
	map<string, SceneLight*>::iterator lightIterator = rootVertex->lights.begin();
	for(int i = 0; lightIterator != rootVertex->lights.end(); lightIterator++, i++) {
		if(i%4 == 0)
			addColumnToPanel(lightsPanel);

		string lightStr = "Light " + lightIterator->first;
		addCheckboxToPanel(lightsPanel, (char*) lightStr.c_str(), &(rootVertex->lightOnControls.find(lightIterator->first)->second), lastID);
		lightMap.insert(pair<int, string>(lastID, lightIterator->first));
		lastID++;
	}

	lightsPanel->align();

	GLUI_Panel* camerasPanel = addPanel((char*)"Cameras");
	GLUI_Listbox* cameraList = addListboxToPanel(camerasPanel, (char*)"Camera: ", &((DemoScene*) scene)->activeCameraNum, lastID);
	lastID++;
	map<string, CameraView*>::iterator cameraIterator = rootVertex->cameras.begin();
	cameraList->add_item(0, (char*)"Default");
	for(int i = 1; cameraIterator != rootVertex->cameras.end(); cameraIterator++, i++) {
		cameraList->add_item(i, (char*)cameraIterator->first.c_str());
		if(cameraIterator->first == YAFCamera::initialCameraID)
			cameraList->set_int_val(i);
	}

	camResetButton = addButtonToPanel(camerasPanel, (char*)"Reset camera view", lastID);
	camResetButtonID = lastID;
	lastID++;
	camerasPanel->align();

	GLUI_Checkbox* displayListsCheckbox = addCheckbox("Use display lists", NULL, lastID);
	if(sceneGraph->drawDisplayLists == true)
		displayListsCheckbox->set_int_val(1);
	else
		displayListsCheckbox->set_int_val(0);

	displayListCheckboxID = lastID;
	lastID++;

	addColumn();

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

	GLUI_Panel* animationsPanel = addPanel((char*) "Animations");
	GLUI_Listbox* animationList = addListboxToPanel(animationsPanel, (char*)"Animation: ", &selectedAnimation, lastID);
	animationListboxID = lastID;
	lastID++;
	animationsExist = !sceneGraph->animations.empty();
	if(!animationsExist) {
		animationList->add_item(1, "No animations");
		animationList->disable();
	}
	else {
		map<string, Animation*>::iterator animationIterator = sceneGraph->animations.begin();
		for(int i = 1; animationIterator != sceneGraph->animations.end(); animationIterator++, i++) {
			animationList->add_item(i, (char*)animationIterator->first.c_str());
			animationMap.insert(pair<int, string>(i, animationIterator->first));
		}
	}

	animationLoopCheckbox = addCheckboxToPanel(animationsPanel, "Loop", NULL , lastID);
	int isLoop = 0;
	if(animationsExist) {
		if(sceneGraph->animationIsLooping(animationMap.find(1)->second) == true ) {
			isLoop = 1;
		}

		animationLoopCheckbox->set_int_val(isLoop);
	}
	else
		animationLoopCheckbox->disable();

	animationLoopCheckboxID = lastID;
	lastID++;

	GLUI_Button* animationResetButton = addButtonToPanel(animationsPanel, (char*)"Reset", lastID);
	animationResetButtonID = lastID;
	if(!animationsExist)
		animationResetButton->disable();
	lastID++;

	string pauseLabel = "Pause";
	animationPaused = false;
	if(animationsExist) {
		if(sceneGraph->animationIsPaused(animationMap.find(1)->second) == true) {
			pauseLabel = "Start";
			animationPaused = true;
		}
	}
	animationPauseButton = addButtonToPanel(animationsPanel, (char*)pauseLabel.c_str(), lastID);
	animationPauseButtonID = lastID;
	if(!animationsExist)
		animationPauseButton->disable();
	lastID++;

	GLUI_Button* allAnimationResetButton = addButtonToPanel(animationsPanel, (char*)"Reset all animations", lastID);
	allAnimationResetButtonID = lastID;
	if(!animationsExist)
		allAnimationResetButton->disable();
	lastID++;

	animationsPanel->align();

	addColumn();

	GLUI_Panel* waterlinesPanel = addPanel((char*) "Water lines");
	GLUI_Spinner* speedSpinner = addSpinnerToPanel(waterlinesPanel, "Water speed", GLUI_SPINNER_INT, &sceneGraph->currentShaderSpeedControl, lastID);
	speedSpinner->set_int_limits(-500, 500, GLUI_LIMIT_CLAMP);
	speedSpinnerID = lastID;
	lastID++;
	GLUI_Spinner* heightSpinner = addSpinnerToPanel(waterlinesPanel, "Wave height", GLUI_SPINNER_INT, &sceneGraph->currentShaderHeightControl, lastID);
	heightSpinner->set_int_limits(1, 200, GLUI_LIMIT_CLAMP);
	heightSpinnerID = lastID;
	lastID++;
	GLUI_Spinner* inclineSpinner = addSpinnerToPanel(waterlinesPanel, "Wave incline", GLUI_SPINNER_INT, &sceneGraph->currentShaderInclineControl, lastID);
	inclineSpinner->set_int_limits(-100, 100, GLUI_LIMIT_CLAMP);
	inclineSpinnerID = lastID;
	lastID++;

}

void RendererInterface::processGUI(GLUI_Control *ctrl) {
	RootVertex* rootVertex = sceneGraph->getRootVertex();

	if( ((DemoScene*) scene)->activeCameraNum == 0) {
		camResetButton->disable();
	}
	else
		camResetButton->enable();

	map<int, string>::iterator lightToToggle = lightMap.find(ctrl->user_id);
	if(lightToToggle != lightMap.end()) {
		if (!rootVertex->lightOnControls.find(lightToToggle->second)->second){
			rootVertex->lights.find(lightToToggle->second)->second->disable();
		} else {
			rootVertex->lights.find(lightToToggle->second)->second->enable();
		}
		return;
	}

	if(ctrl->user_id == camResetButtonID && camResetButton->enabled) {
		((DemoScene*) scene)->resetCurrentCamera();
	}

	if(ctrl->user_id == animationListboxID) {
		int isLoop = 0;
		if(sceneGraph->animationIsLooping(animationMap.find(selectedAnimation)->second) == true ) {
			isLoop = 1;
		}

		animationLoopCheckbox->set_int_val(isLoop);

		if(sceneGraph->animationIsPaused(animationMap.find(selectedAnimation)->second) == true ) {
			animationPauseButton->set_name("Start");
			animationPaused = true;
		}
		else {
			animationPauseButton->set_name("Pause");
			animationPaused = false;
		}
	}

	if(ctrl->user_id == animationLoopCheckboxID) {
		map<int, string>::iterator animationToToggle = animationMap.find(selectedAnimation);
		if(animationLoopCheckbox->get_int_val() == 1) {
			sceneGraph->setAnimationLoop(animationToToggle->second, true);
		}
		else
			sceneGraph->setAnimationLoop(animationToToggle->second, false);
	}

	if(ctrl->user_id == animationResetButtonID) {
		map<int, string>::iterator animationToToggle = animationMap.find(selectedAnimation);
		
		//If the animation isn't looping, pressing reset will pause it on the initial point
		if(!sceneGraph->animationIsLooping(animationToToggle->second) ) {
			animationPauseButton->set_name("Start");
			animationPaused = true;
		}
		//If the animation is looping and is initially paused when we press reset, it's going to unpause itself, so we need to toggle the button
		else if(sceneGraph->animationIsLooping(animationToToggle->second) && sceneGraph->animationIsPaused(animationToToggle->second)) {
			animationPauseButton->set_name("Pause");
			animationPaused = false;
		}

		sceneGraph->resetAnimation(animationToToggle->second);
	}

	if(ctrl->user_id == allAnimationResetButtonID) {
		map<int, string>::iterator animationToToggle = animationMap.find(selectedAnimation);

		//If the animation isn't looping, pressing reset will pause it on the initial point
		if(!sceneGraph->animationIsLooping(animationToToggle->second) ) {
			animationPauseButton->set_name("Start");
			animationPaused = true;
		}
		//If the animation is looping and is initially paused when we press reset, it's going to unpause itself, so we need to toggle the button
		else if(sceneGraph->animationIsLooping(animationToToggle->second) && sceneGraph->animationIsPaused(animationToToggle->second)) {
			animationPauseButton->set_name("Pause");
			animationPaused = false;
		}

		sceneGraph->resetAllAnimations();
	}

	if(ctrl->user_id == animationPauseButtonID) {
		map<int, string>::iterator animationToToggle = animationMap.find(selectedAnimation);
		if(animationPaused == true) {
			sceneGraph->resumeAnimation(animationToToggle->second);
			animationPaused = false;
			animationPauseButton->set_name("Pause");
		}
		else {
			sceneGraph->pauseAnimation(animationToToggle->second);
			animationPaused = true;
			animationPauseButton->set_name("Start");
		}
	}

	if(ctrl->user_id == speedSpinnerID || ctrl->user_id == heightSpinnerID || ctrl->user_id == inclineSpinnerID) {
		//Inform the graph that the shader scales need to be updated on the next scene update
		sceneGraph->shaderScalesUpdated = true;
	}

	if(ctrl->user_id == displayListCheckboxID) {
		sceneGraph->drawDisplayLists = !sceneGraph->drawDisplayLists;
	}
}

void RendererInterface::processMouse(int button, int state, int x, int y) {
	CGFinterface::processMouse(button,state, x, y);

	// do picking on mouse press (GLUT_DOWN)
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		performPicking(x,y);

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		Game* game = sceneGraph->getGame();
		game->setSelectState(SELECT_ANY);
		printf("Changed state to select any\n");
		game->selectedPieceID = 0;
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
		// this should be replaced by code handling the picked object's ID's (stored in "selected"), 
		// possibly invoking a method on the scene class and passing "selected" and "nselected"

		for(unsigned int i = 0; i < nselected; i++)
			printf("Square selected: %d\n", selected[i]);

		Game* game = sceneGraph->getGame();
		printf("state: %d\n", game->getSelectState());
		if(game->getSelectState() == SELECT_ANY) {
			if(game->isBoardPiece(selected[0]) && game->isOwnPiece(selected[0])) {
				game->selectedPieceID = selected[0];
				game->setSelectState(SELECT_TO_SQUARE);
				printf("Changed state to to square\n");
			}
			else {
				printf("Can't select that piece\n");
			}
		}
		else if(game->getSelectState() == SELECT_TO_SQUARE) {
			unsigned int selectedPosition = selected[0];
			if(game->canMoveTo(selectedPosition)) {
				printf("Changed state to select any\n");
				game->setSelectState(SELECT_ANY);

				PositionPoint origin = game->getBoardPiecePosition(game->selectedPieceID);
				PositionPoint destination = game->getPickingSquarePosition(selectedPosition);
				sceneGraph->movePiece(game->selectedPieceID, origin, destination);
				int squareToRemove = game->executeMove(game->selectedPieceID, destination);
				if(squareToRemove != 0) {
					int removePieceID = game->getPieceOnSquare(squareToRemove);
					BoardPiece* piece = game->getBoardPiece(removePieceID);
					piece->onBoard = false;
					piece->playable = false;
					string removePieceIDStr = game->getPieceIDStr(piece->id);
					PositionPoint removePieceOrigin = game->getPickingSquarePosition(piece->squareID);
					PositionPoint  restPoint = game->getPieceRestPosition(piece);
					sceneGraph->movePiece(removePieceID, removePieceOrigin, restPoint);
					game->setBoardPiecePosition(removePieceID, restPoint);
				}
			}
			else {
				printf("Illegal move!\n");
			}
		}
		else if(game->getSelectState() == SELECT_SECOND_ENEMY) {
			unsigned int selectedPosition = selected[0];
			int capturedPieceID = game->getPieceOnSquare(selectedPosition);
			if(game->canCapture(capturedPieceID)) {
				game->capture(capturedPieceID);
				BoardPiece* capturedPiece = game->getBoardPiece(capturedPieceID);
				PositionPoint origin = game->getBoardPiecePosition(capturedPieceID);
				PositionPoint restPoint = game->getPieceRestPosition(capturedPiece);
				capturedPiece->onBoard = false;
				capturedPiece->playable = false;
				sceneGraph->movePiece(capturedPieceID, origin, restPoint);
				game->setBoardPiecePosition(capturedPieceID, restPoint);
			}
			else {
				printf("Can't eat that piece\n");
			}
		}
	}
	else
		printf("Nothing selected while picking \n");	
}


