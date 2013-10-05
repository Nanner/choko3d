#include "DemoScene.h"
#include "CGFaxis.h"
#include "CGFapplication.h"

#include <math.h>

#include "CGFappearance.h"

void DemoScene::init() 
{
	//Configure the different general scene attributes (background, cameras, lights, etc)
	sceneGraph->configureScene();

	//Get the initial camera
	CGFscene::activeCamera = sceneGraph->getInitialCamera();

	setUpdatePeriod(30);
}

void DemoScene::update(unsigned long t)
{}
	
void DemoScene::display() 
{

	// ---- BEGIN Background, camera and axis setup
	
	// Clear image and depth buffer everytime we update the scene
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Initialize Model-View matrix as identity (no transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Apply transformations corresponding to the camera position relative to the origin
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

DemoScene::~DemoScene()
{
	delete(sceneGraph);
}
