#ifndef _SCENEVERTEX_H
#define _SCENEVERTEX_H

#include "SceneEdge.h"
#include "YAFGlobal.h"
#include "CGFscene.h"
#include <vector>

using namespace std;

class SceneGraph;

class SceneVertex {
	vector<SceneEdge> adj;
	bool childVisited;
	bool nodeVisited;

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
	virtual void render() {}
};

class RootVertex: public SceneVertex {
public:
	YAFGlobal globals;

	RootVertex(float* matrix, string id, YAFGlobal globals);

	void setGlobals();
};

class SceneComposite: public SceneVertex {
public:
	SceneComposite(float* matrix, string id);
};


#endif
