#include "PrologBridge.h"
#include "DemoScene.h"
#include "GameState.h"

DemoScene::DemoScene(SceneGraph* sceneGraph, RendererInterface* rendererInterface): sceneGraph(sceneGraph), rendererInterface(rendererInterface) {}

void DemoScene::init() 
{
	restartGameOnUpdate = false;
	filmMode = false;
	filmStarted = false;

	//Configure the different general scene attributes (background, lights, etc)
	sceneGraph->configureScene();
	initCameras();
	setUpdatePeriod(16);
	isSelectMode = false;
    
	float ambient[4] = {0.0, 1.0, 0.0, 0.4};
	float diffuse[4] = {0.0, 1.0, 0.0, 0.4};
	float specular[4] = {0.0, 0.0, 0.0, 0.4};
	float emissivity[4] = {0.0, 0.0, 0.0, 0.0};
	float shininess = 0.0;

	defaultHighlightedSquareAppearance = new Appearance(ambient, diffuse, specular, emissivity, shininess);

	defaultHighlightedSquare = new Plane(10);
	defaultHighlightedSquare->setAppearance(defaultHighlightedSquareAppearance);
    
    game = sceneGraph->getGame();
    game->setCameraController(&cameraController);
}

void DemoScene::initCameras() {
	scene_cameras.clear();
	scene_cameras.push_back(cameraController.getFreeCamera());
	scene_cameras.push_back(cameraController.getAutoCamera());
	activeCameraNum = AUTO_CAMERA;
}

void DemoScene::update(unsigned long t){
	if(cameraController.isChangingFocus) {
		if(cameraController.focusChangeInitialized)
			cameraController.updateFocus(t);
		else {
			cameraController.initializeFocusChange(t);
        }
	}
    
	if(!isSelectMode) {
		map<string, Animation*>::iterator animationItr = sceneGraph->animations.begin();
		for(; animationItr != sceneGraph->animations.end(); animationItr++) {
			Animation * animation = animationItr->second;
			if (animation->isInitialized)
				animation->update(t);
			else
				animation->init(t);
		}

		if(sceneGraph->hasShader) {
			if(sceneGraph->shaderScalesUpdated) {
				sceneGraph->updateWaterShaderScales();
				sceneGraph->shaderScalesUpdated = false;
			}

			sceneGraph->updateShaders(t);
		}

		PieceAnimation::updatePieceAnimations(t);
	}
    
    game->update(t);
    
    if((game->currentPlayerIsAI() || game->onSkippedTurn)
       && !PieceAnimation::pendingAnimations()
       && !game->hasGameEnded()
       && !filmMode
       && !game->AIisStandingBy
       && !cameraController.isChangingFocus
       && game->movesPossible) {

		if(game->onSkippedTurn)
			rendererInterface->deselectCurrentPiece();

		game->updateAI();
		sceneGraph->animateAIPlay(game->getGameState().getMove());
		game->processAIMovedPieces(game->getGameState().getMove());
	}
    
	if(PieceAnimation::pendingAnimations() || game->getSelectState() == SELECT_SECOND_ENEMY || filmMode || cameraController.isChangingFocus) {
		rendererInterface->undoButton->disable();
	}
	else if(!rendererInterface->undoButton->enabled) {
		rendererInterface->undoButton->enable();
	}

	if(cameraController.isChangingFocus) {
		rendererInterface->cameraRotationCheckbox->disable();
	}
	else if(!rendererInterface->cameraRotationCheckbox->enabled) {
		rendererInterface->cameraRotationCheckbox->enable();
	}
    
	if (game->hasGameEnded()
        && !PieceAnimation::pendingAnimations()
        && !filmMode
        && !cameraController.isChangingFocus
        && !restartGameOnUpdate) {
		rendererInterface->updateGameOver();
		return;
	}

	if (!PieceAnimation::pendingAnimations()
        && filmMode
        && filmEnded
        && !cameraController.isChangingFocus
        && !restartGameOnUpdate) {
		rendererInterface->updateFilmOver();
		return;
	}

	if (!game->movesPossible
		&& !PieceAnimation::pendingAnimations()
		&& !cameraController.isChangingFocus)
		rendererInterface->updateNoMoves();
}

void DemoScene::display() 
{
	setDrawMode(activeDrawMode);
	// ---- BEGIN Background, camera and axis setup

	// Clear image and depth buffer everytime we update the scene
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Initialize Model-View matrix as identity (no transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Apply transformations corresponding to the camera position relative to the origin
	activeCameraNum = cameraController.getEnabledCameraType();
	activateCamera(activeCameraNum);
	CGFscene::activeCamera->applyView();

	// Draw (and update) lights
	//sceneGraph->drawLights();
    // with multiple scenes, we must update all lights
    // because turned off lights must still be updated
    sceneGraph->drawAllLights();

	// Draw axis
    glColor3f(1.0,1.0,1.0);
	//axis.draw();


	// ---- END Background, camera and axis setup


	// ---- BEGIN feature demos
	if(restartGameOnUpdate) {
		restartGameOnUpdate = false;
		sceneGraph->restartGame();
	}

	if(filmMode) {
		if(filmStarted && !PieceAnimation::pendingAnimations() && !filmGameStates.empty() && !cameraController.isChangingFocus) {
			sceneGraph->animateAIPlay(filmGameStates.top().parsedMove);
			sceneGraph->getGame()->processAIMovedPieces(filmGameStates.top().parsedMove);
			sceneGraph->getGame()->addGameState(filmGameStates.top());
			filmGameStates.pop();
			if(filmGameStates.empty()) {
				filmStarted = false;
				filmEnded = true;
			}
		}
	}

	if(!isSelectMode) {
		sceneGraph->render();
	}
	else {
		sceneGraph->renderPickingSquares();
	}

	sceneGraph->renderBoardPieces();
    
    drawHighlightedSquares();

    //drawHUD();
    
	// ---- END feature demos

	// We have been drawing in a memory area that is not visible - the back buffer, 
	// while the graphics card is showing the contents of another buffer - the front buffer
	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();
}

void DemoScene::drawHighlightedSquares() {
    vector<PositionPoint> highlightedSquares = sceneGraph->getGame()->getHighlightedSquarePositions();

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i = 0 ; i < highlightedSquares.size(); i++) {
        PositionPoint selectedSquare = highlightedSquares.at(i);
        glPushMatrix();
		glTranslatef(selectedSquare.x, selectedSquare.y + 0.1, selectedSquare.z);
		glScalef(3.1, 3.1, 3.1);
        if (sceneGraph->highlightedSquare == NULL) {
            defaultHighlightedSquareAppearance->apply();
            defaultHighlightedSquare->draw();
        } else {
            sceneGraph->render(sceneGraph->highlightedSquare);
        }
		glPopMatrix();
    }

	glDisable(GL_BLEND);
}

void DemoScene::drawHUD() {
    glDisable(GL_LIGHTING);
    
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	// Switch the view modes to an Ortho style
	// this will allow us to see staight on what is rendered following.
	glOrtho( 0.0, 800, 0.0, 600, -1.0, 1.0 );
	glMatrixMode( GL_MODELVIEW );
    
	glPushMatrix();
	glLoadIdentity();
    
	glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, hudImage);
    hudAppearance->apply();
    
	glColor3f(1, 1, 1);
    
	glBegin(GL_QUADS);
    
	// Draw the quad at the appropriate area
	// the Texture is drawn over it.
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0,  0);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 800, 0,  0);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 800,  70,  0);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0,  70,  0);	// Top Left Of The Texture and Quad
    
	glEnd();
    
	glDisable(GL_TEXTURE_2D);
    
    glColor3f(0.5f, 0.5f, 0.5f);
    glRasterPos2f(0, 0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'c');
    
	//Unsets Projection Mode
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
    
	glEnable(GL_LIGHTING);
}

void DemoScene::setDrawMode(int mode) {
	switch(mode) {
	case DRAWMODE_FILL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case DRAWMODE_LINE:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case DRAWMODE_POINT:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	default:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
}

void DemoScene::resetCurrentCamera() {
	((CameraView*) CGFscene::activeCamera)->resetCamera();
}

void DemoScene::restartGameOnNextUpdate() {
	restartGameOnUpdate = true;
	filmMode = false;
}

void DemoScene::startFilmMode() {
	stack<GameState> tmp = sceneGraph->getGame()->getGameStates();

	while(!filmGameStates.empty()) {
		filmGameStates.pop();
	}

	while(!tmp.empty()) {
		filmGameStates.push(tmp.top());
		tmp.pop();
	}
	restartGameOnNextUpdate();
	filmMode = true;
	filmEnded = false;
	filmStarted = true;
}


DemoScene::~DemoScene()
{
	delete(sceneGraph);
}
