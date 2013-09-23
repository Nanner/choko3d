#ifndef _SCENEEDGE_H
#define _SCENEEDGE_H

//#include "SceneGraph.h"
//#include "SceneVertex.h"

class SceneGraph;
class SceneVertex;

class SceneEdge {
	SceneVertex *dest;
	double weight;
public:
	SceneEdge(SceneVertex *d, double w): dest(d), weight(w){}
	friend class SceneGraph;
	friend class SceneVertex;
};

#endif