#ifndef _SCENEVERTEX_H
#define _SCENEVERTEX_H

#include "SceneEdge.h"
#include "YAFGlobal.h"
#include "YAFCamera.h"
#include "CGFscene.h"
#include "CameraView.h"
#include <vector>
#include <map>

using namespace std;

class SceneGraph;

class SceneVertex {
	vector<SceneEdge> adj;
	bool childVisited;
	bool nodeVisited;
    CGFappearance * appearance;

protected:
	float* matrix;

public:
	//ScenePrimitive IDs are empty
	string id;

	SceneVertex();
	friend class SceneGraph;

	void addEdge(SceneVertex *dest);
	bool removeEdgeTo(SceneVertex  *d);

	bool operator<(const SceneVertex vertex);

	vector<SceneVertex*> getEdges();

	float* getMatrix();
    
    CGFappearance * getAppearance();
    void setAppearance(CGFappearance * appearance);
    
	virtual void render() {}
};

class RootVertex: public SceneVertex {
public:
	YAFGlobal globals;

	map<string, CameraView*> cameras;

	RootVertex(float* matrix, string id, YAFGlobal globals, map<string, YAFCamera> cameras);

	void setGlobals();
	void setInitialCamera();
};

class SceneComposite: public SceneVertex {
public:
	SceneComposite(float* matrix, string id);
};


#endif
