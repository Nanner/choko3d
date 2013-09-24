#include "SceneVertex.h"

SceneVertex::SceneVertex(/*put info here!*/): nodeVisited(false), childVisited(false) {}

void SceneVertex::addEdge(SceneVertex *dest) {
	SceneEdge edgeD(dest);
	adj.push_back(edgeD);
}

bool SceneVertex::removeEdgeTo(SceneVertex *d) {
	vector<SceneEdge >::iterator it= adj.begin();
	vector<SceneEdge >::iterator ite= adj.end();
	while (it!=ite) {
		if (it->dest == d) {
			adj.erase(it);
			return true;
		}
		else it++;
	}
	return false;
}

vector<SceneVertex*> SceneVertex::getEdges() {
	vector<SceneEdge>::iterator it= adj.begin();
	vector<SceneEdge>::iterator ite= adj.end();
	vector<SceneVertex*> edges;
	while (it!=ite) {
		edges.push_back( it->dest );
		++it;
	}
	return edges;
}

float* SceneVertex::getMatrix() {
	return matrix;
}

SceneComposite::SceneComposite(float* matrix) {
	this->matrix = matrix;
}