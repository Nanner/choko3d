#ifndef _SCENEGRAPH_H
#define _SCENEGRAPH_H

#include "CGFobject.h"
#include "SceneVertex.h"
#include "SceneEdge.h"
#include "YAFReader.h"
#include "Appearance.h"
#include <vector>

using namespace std;

class SceneGraph {
	vector<SceneVertex *> vertexSet;
	RootVertex* rootVertex;
    map<string, Appearance *> appearances;

public:
	SceneGraph(){}
	SceneGraph(YAFReader* yafFile);
	~SceneGraph();
	bool addVertex(SceneVertex *in);
	bool addEdge(SceneVertex *sourc, SceneVertex *dest);
	bool removeVertex(SceneVertex *in);
	bool removeEdge(SceneVertex *sourc, SceneVertex *dest);
	
	vector<SceneVertex * > getVertexSet() const;
	unsigned long getNumVertex() const;

	void processRootNode(YAFNode root, YAFReader* yafFile);
	void processYAFNode(YAFNode yafNode);
	void loadVertexPrimitives(vector<ScenePrimitive*> primitives, SceneVertex* vertex);
	void processYAFNodeReferences(YAFNode yafNode);

	void configureScene();
	CGFcamera* getInitialCamera();
	void drawLights();

	void render(SceneVertex *v);
	void render();
};

#endif