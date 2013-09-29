#ifndef _SCENEVERTEX_H
#define _SCENEVERTEX_H

#include "SceneEdge.h"
#include "YAFGlobal.h"
#include "YAFCamera.h"
#include "CGFscene.h"
#include "CameraView.h"
#include "Appearance.h"
#include "YAFLight.h"
#include "SceneLight.h"
#include <vector>
#include <map>

using namespace std;

class SceneGraph;

class SceneVertex {
	vector<SceneEdge> adj;
	bool childVisited;
	bool nodeVisited;
    Appearance * appearance;
	bool inheritedAppearance;

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
    
    Appearance * getAppearance();
    void setAppearance(Appearance * appearance);
    
	virtual void render() {}
};

class RootVertex: public SceneVertex {
public:
	CGFappearance* defaultAppearance;
	YAFGlobal globals;

	map<string, CameraView*> cameras;
	map<string, SceneLight*> lights;

	RootVertex(float* matrix, string id, YAFGlobal globals, map<string, YAFCamera> cameras, map<string, YAFLight> lights);

	void loadDefaultAppearance();
	void setGlobals();
	void setInitialCamera();
};

class SceneComposite: public SceneVertex {
public:
	SceneComposite(float* matrix, string id);
};


#endif
