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
#include "RendererInterface.h"
#include <math.h>

class RendererInterface;

class DemoScene : public CGFscene
{
public:
	int activeCameraNum;
	int activeDrawMode;
	bool isSelectMode;
	bool filmMode;
	bool filmStarted;
	bool filmEnded;
	stack<GameState> filmGameStates;

	void init();
	void initCameras();
	void display();
	void update(unsigned long t);
	void setDrawMode(int mode);
	void resetCurrentCamera();
    void setSceneGraph();
	SceneGraph* getSceneGraph(){return sceneGraph;}
	DemoScene(SceneGraph* sceneGraph, RendererInterface* rendererInterface, CGFapplication * app);
	void recreateSceneGraph();
    void drawHUD();
	void restartGameOnNextUpdate();

	void startFilmMode();
	~DemoScene();
private:
    CGFapplication * app;
	bool restartGameOnUpdate;
	SceneGraph* sceneGraph;
	Appearance* squareSelectionAppearance;
    CGFappearance* hudAppearance;
	Plane* squareSelection;
    RendererInterface * rendererInterface;
};

#endif