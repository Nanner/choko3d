#ifndef _SCENEGRAPH_H
#define _SCENEGRAPH_H

#include "CGFobject.h"
#include "SceneVertex.h"
#include "SceneEdge.h"
#include <vector>

using namespace std;

class SceneGraph {
	vector<SceneVertex *> vertexSet;

public:
	~SceneGraph();
	bool addVertex(SceneVertex *in);
	bool addEdge(SceneVertex *sourc, SceneVertex *dest);
	bool removeVertex(SceneVertex *in);
	bool removeEdge(SceneVertex *sourc, SceneVertex *dest);
	
	vector<SceneVertex * > getVertexSet() const;
	unsigned long getNumVertex() const;

	//TODO fix dfsVisit to draw and pushPop stuffs
	void dfsVisit(SceneVertex *v);
	void dfsVisit();
};

#endif