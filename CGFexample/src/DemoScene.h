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
#include "CameraController.h"

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
	SceneGraph* getSceneGraph(){return sceneGraph;}
	DemoScene(SceneGraph* sceneGraph, RendererInterface* rendererInterface);
	void recreateSceneGraph();
    void drawHUD();
	void restartGameOnNextUpdate();

	void startFilmMode();
	~DemoScene();
    void drawHighlightedSquares();
private:
	bool restartGameOnUpdate;
	SceneGraph* sceneGraph;
	Appearance* defaultHighlightedSquareAppearance;
    CGFappearance* hudAppearance;
	Plane* defaultHighlightedSquare;
    RendererInterface * rendererInterface;
	CameraController cameraController;
    Game * game;
};

#endif