#include "DemoScene.h"
#include "CGFaxis.h"
#include "CGFapplication.h"

#include <math.h>

#include "CGFappearance.h"

void DemoScene::init() 
{
	//Configure the different general scene attributes (background, lights, etc)
	sceneGraph->configureScene();

	//Configure cameras
	initCameras();
	

	setUpdatePeriod(30);
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

void DemoScene::update(unsigned long t)
{}
	
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
	sceneGraph->render();

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
