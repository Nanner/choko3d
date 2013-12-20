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
#include "CGFshader.h"
#include "WaterShader.h"
#include "Game.h"
#include "LinearAnimation.h"
#include "CurvedAnimation.h"
#include "PieceAnimation.h"
#include <vector>
#include <stack>
#include <map>
#include <queue>

#define MOVE_DURATION 1.0

using namespace std;

class Animation;
class PieceAnimation;
class SceneGraph;

class SceneVertex {
	vector<SceneEdge> adj;
	bool childVisited;
	bool nodeVisited;
    Appearance * appearance;
	bool inheritedAppearance;
    map<pair<string, unsigned int>, unsigned int> displayLists;
	map<pair<string, unsigned int>, bool> initializedDisplayLists;
	map<pair<string, unsigned int>, bool> displayListsInStack;
    
    static unsigned int currentDisplayList;

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
	void setMatrix(float matrix[]);
    
    Appearance * getAppearance();
    void setAppearance(Appearance * appearance);
    
	virtual void draw() {}
    
    bool usesDisplayList;
	void initializeDisplayList(pair<string, unsigned int> vertexAppearance);
    bool initializedDisplayList(pair<string, unsigned int> vertexAppearance);

	bool displayListInStack(pair<string, unsigned int> vertexAppearance);
	void putInStack(pair<string, unsigned int> vertexAppearance);

    void activateDisplayList();
    void disableDisplayList();
    unsigned int getDisplayList(pair<string, unsigned int> vertexAppearance);
    
    Animation * animation;
    
    void setAnimation(Animation * animation);
    Animation * getAnimation();
	virtual WaterShader* getShader() {return NULL;}
	void createMovementAnimation(PositionPoint p1, PositionPoint p2);

	queue<PieceAnimation*> pieceAnimations;
	void applyPieceAnimation();
};


class SceneComposite: public SceneVertex {
public:
	SceneComposite(float* matrix, string id);
};


#endif
