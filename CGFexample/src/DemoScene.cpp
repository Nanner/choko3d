#include "DemoScene.h"

DemoScene::DemoScene(SceneGraph* sceneGraph):sceneGraph(sceneGraph){}

void DemoScene::init() 
{
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
}

void DemoScene::initCameras() {
	scene_cameras.clear();

	CGFcamera* cgfDefault = new CGFcamera();
	cgfDefault->moveTo(25,0,0);
	cgfDefault->rotate(0,20);
	cgfDefault->rotate(1,-45);
	scene_cameras.push_back(cgfDefault);

	map<string, CameraView*>::iterator it = sceneGraph->getRootVertex()->cameras.begin();
	for(int i = 1; it != sceneGraph->getRootVertex()->cameras.end(); it++, i++) {
		scene_cameras.push_back(it->second);
		if(it->first == YAFCamera::initialCameraID)
			activeCameraNum = i;
	}
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
	if(!isSelectMode) {
		sceneGraph->render();
	}
	else {
		sceneGraph->renderPickingSquares();
	}

	sceneGraph->renderBoardPieces();

	PositionPoint selectedSquare = sceneGraph->getGameState()->getSelectedSquarePosition();
	//if p = {0, 0, 0}, no square is selected
	if(selectedSquare.x != 0 || selectedSquare.y != 0 || selectedSquare.z != 0) {
		glPushMatrix();
		glTranslatef(selectedSquare.x, selectedSquare.y + 0.1, selectedSquare.z);
		glScalef(3, 3, 3);
		squareSelectionAppearance->apply();
		squareSelection->draw();
		glPopMatrix();
	}

	// ---- END feature demos

	// We have been drawing in a memory area that is not visible - the back buffer, 
	// while the graphics card is showing the contents of another buffer - the front buffer
	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();
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

DemoScene::~DemoScene()
{
	delete(sceneGraph);
}
