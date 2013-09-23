#ifndef _SCENEVERTEX_H
#define _SCENEVERTEX_H

#include <vector>

using namespace std;

class SceneGraph;
class SceneEdge;

class SceneVertex {
	vector<SceneEdge> adj;
	bool visited;
public:

	SceneVertex();
	friend class SceneGraph;

	void addEdge(SceneVertex *dest);
	bool removeEdgeTo(SceneVertex  *d);

	bool operator<(const SceneVertex vertex);

	vector<SceneEdge> getEdges() ;
};

class SceneComposite: public SceneVertex {

};

class ScenePrimitive: public SceneVertex {

};


#endif
