#ifndef DEMOSCENE_H
#define DEMOSCENE_H

#include "CGFscene.h"
#include "CGFshader.h"
#include "SceneGraph.h"

class DemoScene : public CGFscene
{
public:
	void init();
	void display();
	void update(unsigned long t);
	DemoScene(SceneGraph* sceneGraph):sceneGraph(sceneGraph){}
	~DemoScene();
private:
	CGFlight* light0;
	CGFobject* obj;
	CGFappearance* materialAppearance;
	CGFappearance* textureAppearance;
	CGFshader* shader;
	SceneGraph* sceneGraph;
};

#endif