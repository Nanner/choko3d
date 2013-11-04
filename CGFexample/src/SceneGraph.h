#ifndef _SCENEGRAPH_H
#define _SCENEGRAPH_H

#include "CGFobject.h"
#include "SceneVertex.h"
#include "RootVertex.h"
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
    map<string, Animation *> animations;
	stack<int> displayListOrder;
	bool stackReady;

	bool hasShader;
	vector<CGFshader* > currentShaders;

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

	RootVertex* getRootVertex();

	void configureScene();
	CGFcamera* getInitialCamera();
	void drawLights();

	void render(SceneVertex *v);
	void render();

	//This function searches the graph to see if any shaders are used, adding them to the shader vector
	//And toggling the hasShader bool to true. This is needed for the demoscene update function
	void findShaders();

	//This function updates the shaders with a time t
	void updateShaders(unsigned long t);
};

#endif