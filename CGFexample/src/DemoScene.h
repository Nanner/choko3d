#ifndef DEMOSCENE_H
#define DEMOSCENE_H

#define DRAWMODE_FILL 0
#define DRAWMODE_LINE 1
#define DRAWMODE_POINT 2

#include "CGFscene.h"
#include "CGFshader.h"
#include "SceneGraph.h"

class DemoScene : public CGFscene
{
public:
	int activeCameraNum;
	int activeDrawMode;

	void init();
	void initCameras();
	void display();
	void update(unsigned long t);
	void setDrawMode(int mode);
	SceneGraph* getSceneGraph(){return sceneGraph;}
	DemoScene(SceneGraph* sceneGraph):sceneGraph(sceneGraph){}
	~DemoScene();
private:
	SceneGraph* sceneGraph;
};

#endif