#ifndef DEMOSCENE_H
#define DEMOSCENE_H

#define DRAWMODE_FILL 0
#define DRAWMODE_LINE 1
#define DRAWMODE_POINT 2

#include "CGFscene.h"
#include "CGFshader.h"
#include "SceneGraph.h"
#include "CGFappearance.h"
#include "CGFaxis.h"
#include "CGFapplication.h"
#include <math.h>

class DemoScene : public CGFscene
{
public:
	int activeCameraNum;
	int activeDrawMode;
	bool isSelectMode;

	void init();
	void initCameras();
	void display();
	void update(unsigned long t);
	void setDrawMode(int mode);
	void resetCurrentCamera();
	SceneGraph* getSceneGraph(){return sceneGraph;}
	DemoScene(SceneGraph* sceneGraph);
    void drawHUD();
	~DemoScene();
private:
	SceneGraph* sceneGraph;
	Appearance* squareSelectionAppearance;
    CGFappearance* hudAppearance;
	Plane* squareSelection;
};

#endif