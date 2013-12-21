#include "PrologBridge.h"
#include "DemoScene.h"
#include "GameState.h"

DemoScene::DemoScene(YAFReader* yafFile, SceneGraph* sceneGraph, RendererInterface* rendererInterface):yafFile(yafFile), sceneGraph(sceneGraph), rendererInterface(rendererInterface) {}

void DemoScene::init() 
{
	restartGameOnUpdate = false;
	filmMode = false;
	filmStarted = false;

	//Configure the different general scene attributes (background, lights, etc)
	sceneGraph->configureScene();
	initCameras();
	setUpdatePeriod(30);
	isSelectMode = false;
     
	float ambient[4] = {1.0, 1.0, 1.0, 1.0};
	float diffuse[4] = {1.0, 1.0, 1.0, 1.0};
	float specular[4] = {0.0, 0.0, 0.0, 1.0};
	float emissivity[4] = {0.0, 0.0, 0.0, 0.0};
	float shininess = 0.0;

	squareSelectionAppearance = new Appearance(ambient, diffuse, specular, emissivity, shininess);

	squareSelection = new Plane(10);
	squareSelection->setAppearance(squareSelectionAppearance);
    
    hudAppearance = new CGFappearance("hud.png", GL_REPEAT, GL_REPEAT);
    
}

void DemoScene::initCameras() {
	scene_cameras.clear();

	CGFcamera* cgfDefault = new CGFcamera();
	cgfDefault->moveTo(25,0,0);
	cgfDefault->rotate(0,20);
	cgfDefault->rotate(1,-45);
	scene_cameras.push_back(cgfDefault);
/*
	map<string, CameraView*>::iterator it = sceneGraph->getRootVertex()->cameras.begin();
	for(int i = 1; it != sceneGraph->getRootVertex()->cameras.end(); it++, i++) {
		scene_cameras.push_back(it->second);
		if(it->first == YAFCamera::initialCameraID)
			activeCameraNum = i;
	}
 */
}

void DemoScene::update(unsigned long t){

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
   
	Game* game = sceneGraph->getGame();
	if(game->currentPlayerIsAI() && !PieceAnimation::pendingAnimations() && !game->hasGameEnded() && !filmMode && !game->AIisStandingBy) {
		game->updateAI();
		sceneGraph->animateAIPlay(game->getGameState().getMove());
		game->processAIMovedPieces(game->getGameState().getMove());
	}

	if(PieceAnimation::pendingAnimations() || game->getSelectState() == SELECT_SECOND_ENEMY || filmMode) {
		rendererInterface->undoButton->disable();
	}
	else if(!rendererInterface->undoButton->enabled) {
		rendererInterface->undoButton->enable();
	}
    
    if (!game->movesPossible)
        rendererInterface->updateNoMoves();
    
    game->update(t);

	if (game->hasGameEnded() && !PieceAnimation::pendingAnimations() && !filmMode) {
		rendererInterface->updateGameOver();
	}

	if (!PieceAnimation::pendingAnimations() && filmMode && filmEnded) {
		rendererInterface->updateFilmOver();
	}
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
	activateCamera(activeCameraNum);
	CGFscene::activeCamera->applyView();

	// Draw (and update) lights
	sceneGraph->drawLights();

	// Draw axis
	axis.draw();


	// ---- END Background, camera and axis setup


	// ---- BEGIN feature demos
	if(restartGameOnUpdate) {
		restartGameOnUpdate = false;
		sceneGraph->restartGame();
	}

	if(filmMode) {
		if(filmStarted && !PieceAnimation::pendingAnimations() && !filmGameStates.empty()) {
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

	PositionPoint selectedSquare = sceneGraph->getGame()->getSelectedSquarePosition();
	//if p = {0, 0, 0}, no square is selected
	if(selectedSquare.x != 0 || selectedSquare.y != 0 || selectedSquare.z != 0) {
		glPushMatrix();
		glTranslatef(selectedSquare.x, selectedSquare.y + 0.1, selectedSquare.z);
		glScalef(3, 3, 3);
		squareSelectionAppearance->apply();
		squareSelection->draw();
		glPopMatrix();
	}
    
    drawHUD();
    
	// ---- END feature demos

	// We have been drawing in a memory area that is not visible - the back buffer, 
	// while the graphics card is showing the contents of another buffer - the front buffer
	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();
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

void DemoScene::recreateSceneGraph() {
	delete sceneGraph;
	sceneGraph = new SceneGraph(yafFile);
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
