#ifndef _SCENEEDGE_H
#define _SCENEEDGE_H

//#include "SceneGraph.h"
//#include "SceneVertex.h"

class SceneGraph;
class SceneVertex;

class SceneEdge {
	SceneVertex *dest;
public:
	SceneEdge(SceneVertex *d): dest(d){}
	friend class SceneGraph;
	friend class SceneVertex;
};

#endif