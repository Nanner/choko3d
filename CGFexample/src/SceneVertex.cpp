#include "SceneVertex.h"

unsigned int SceneVertex::currentDisplayList = 0;

SceneVertex::SceneVertex(): nodeVisited(false), childVisited(false), usesDisplayList(false), animation(NULL) {}

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
    /*currentDisplayList++;
    this->displayList = currentDisplayList;*/
}

void SceneVertex::disableDisplayList() {
    this->usesDisplayList = false;
}

unsigned int SceneVertex::getDisplayList(pair<string, unsigned int> vertexAppearance) {
	map<pair<string, unsigned int>, unsigned int>::iterator it = displayLists.find(vertexAppearance);
	if(it != displayLists.end())
		return it->second;
	else {
		currentDisplayList++;
		displayLists.insert(pair<pair<string, unsigned int>, unsigned int>(vertexAppearance, currentDisplayList));
		initializedDisplayLists.insert(pair<pair<string, unsigned int>, bool>(vertexAppearance, false));
		displayListsInStack.insert(pair<pair<string, unsigned int>, bool>(vertexAppearance, false));
		return currentDisplayList;
	}
}

void SceneVertex::initializeDisplayList(pair<string, unsigned int> vertexAppearance) {
	map<pair<string, unsigned int>, bool>::iterator it = initializedDisplayLists.find(vertexAppearance);
	if(it != initializedDisplayLists.end()) {
		glGenLists(1);
		it->second = true;
	}
}

bool SceneVertex::initializedDisplayList(pair<string, unsigned int> vertexAppearance) {
	map<pair<string, unsigned int>, bool>::iterator it = initializedDisplayLists.find(vertexAppearance);
	if(it != initializedDisplayLists.end())
		return it->second;
	else
		return false;
}

bool SceneVertex::displayListInStack(pair<string, unsigned int> vertexAppearance) {
	map<pair<string, unsigned int>, bool>::iterator it = displayListsInStack.find(vertexAppearance);
	if(it != displayListsInStack.end())
		return it->second;
	else
		return false;
}

void SceneVertex::putInStack(pair<string, unsigned int> vertexAppearance) {
	map<pair<string, unsigned int>, bool>::iterator it = displayListsInStack.find(vertexAppearance);
	if(it != displayListsInStack.end())
		it->second = true;
}

void SceneVertex::setAnimation(Animation * animation){
    this->animation = animation;
}

Animation * SceneVertex::getAnimation(){
    return animation;
}



SceneComposite::SceneComposite(float* matrix, string id) {
	this->matrix = matrix;
	this->id = id;
}