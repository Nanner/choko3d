#ifndef _SCENEGRAPH_H
#define _SCENEGRAPH_H

#include "CGFobject.h"
#include "SceneVertex.h"
#include "RootVertex.h"
#include "SceneEdge.h"
#include "YAFReader.h"
#include "Appearance.h"
#include "Game.h"

#include <vector>

using namespace std;

class SceneGraph {
	vector<SceneVertex *> vertexSet;

	vector<SceneVertex *> pickingSquaresSet;

	vector<SceneVertex* > boardPiecesSet;

	RootVertex* rootVertex;
    map<string, Appearance *> appearances;

	Game* game;

public:
    map<string, Animation *> animations;
	stack<int> displayListOrder;
	bool stackReady;
	bool drawDisplayLists;

	bool hasShader;
	vector<WaterShader* > currentShaders;

	//Tells if there has been some update to the shader control values
	bool shaderScalesUpdated;
	//Current control values for the water shader scales
	int currentShaderSpeedControl;
	int currentShaderHeightControl;
	int currentShaderInclineControl;

	SceneGraph(){}
	SceneGraph(YAFReader* yafFile);
	~SceneGraph();
	bool addVertex(SceneVertex *in, vector<SceneVertex *> &vertexSet);
	bool addEdge(SceneVertex *sourc, SceneVertex *dest, vector<SceneVertex *> &vertexSet);
	bool removeVertex(SceneVertex *in, vector<SceneVertex *> &vertexSet);
	bool removeEdge(SceneVertex *sourc, SceneVertex *dest, vector<SceneVertex *> &vertexSet);

	
	vector<SceneVertex * > getVertexSet() const;
	unsigned long getNumVertex() const;

	void processRootNode(YAFNode root, YAFReader* yafFile);
	void processYAFNode(YAFNode yafNode, vector<SceneVertex *> &vertexSet);
	void loadVertexPrimitives(vector<ScenePrimitive*> primitives, SceneVertex* vertex, vector<SceneVertex *> &vertexSet);
	void processYAFNodeReferences(YAFNode yafNode, vector<SceneVertex *> &vertexSet);

	RootVertex* getRootVertex();

	void configureScene();
	CGFcamera* getInitialCamera();
	void drawLights();

	void render(SceneVertex *v);
	void render();

	void resetAnimation(string id);
	void resetAllAnimations();
	void setAnimationLoop(string id, bool value);
	bool animationIsLooping(string id);
	void pauseAnimation(string id);
	void resumeAnimation(string id);
	bool animationIsPaused(string id);

	//This function searches the graph to see if any shaders are used, adding them to the shader vector
	//And toggling the hasShader bool to true. This is needed for the demoscene update function
	void findShaders();

	//This function updates the shaders with a time t
	void updateShaders(unsigned long t);

	void updateWaterShaderScales();

	Game * getGame();

	void renderPickingSquares();
	void renderPickingSquares(SceneVertex* v);

	void renderBoardPieces();
	void renderBoardPieces(SceneVertex* v);

	void movePiece(unsigned int pieceID, PositionPoint origin, PositionPoint destination);
	void animateAIPlay(Move move);
};

#endif