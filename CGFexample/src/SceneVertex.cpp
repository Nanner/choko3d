#include "SceneVertex.h"

unsigned int SceneVertex::currentDisplayList = 0;

SceneVertex::SceneVertex(): nodeVisited(false), childVisited(false), usesDisplayList(false), displayList(0), initializedDisplayList(false) {}

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

Appearance * SceneVertex::getAppearance() {
    return appearance;
}

void SceneVertex::setAppearance(Appearance * appearance){
    this->appearance = appearance;
}

void SceneVertex::activateDisplayList() {
    this->usesDisplayList = true;
    currentDisplayList++;
    this->displayList = currentDisplayList;
}

void SceneVertex::disableDisplayList() {
    this->usesDisplayList = false;
}

unsigned int SceneVertex::getDisplayList() {
    return displayList;
}


SceneComposite::SceneComposite(float* matrix, string id) {
	this->matrix = matrix;
	this->id = id;
}