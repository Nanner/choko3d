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

void SceneVertex::setMatrix(float matrix[]) {
	this->matrix = matrix;
}

Appearance * SceneVertex::getAppearance() {
    return appearance;
}

void SceneVertex::setAppearance(Appearance * appearance){
    this->appearance = appearance;
}

void SceneVertex::activateDisplayList() {
    this->usesDisplayList = true;
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

void SceneVertex::createMovementAnimation(PositionPoint p1, PositionPoint p2) {
	float origin[3] = {0.0, 0.0, 0.0};
	float destination[3] = {0.0, 0.0, 0.0};

	origin[0] = p1.x; origin[1] = p1.y; origin[2] = p1.z;
	destination[0] = p2.x; destination[1] = p2.y; destination[2] = p2.z;
	vector<float> controlPoints;
	controlPoints.push_back(origin[0]);
    controlPoints.push_back(origin[1]);
    controlPoints.push_back(origin[2]);
    
	controlPoints.push_back(origin[0]);
    controlPoints.push_back(origin[1] + 5.0);
    controlPoints.push_back(origin[2]);
    
	controlPoints.push_back(destination[0]);
    controlPoints.push_back(origin[1] + 5.0);
    controlPoints.push_back(destination[2]);
    
	controlPoints.push_back(destination[0]);
    controlPoints.push_back(destination[1]);
    controlPoints.push_back(destination[2]);

    CurvedAnimation* anim = new CurvedAnimation(MOVE_DURATION, controlPoints);

	//LinearAnimation* anim = new LinearAnimation(MOVE_DURATION, controlPoints);
	if(this->getAnimation() != NULL) {
		glPushMatrix();
		glLoadIdentity();
		glMultMatrixf(this->getAnimation()->getMatrix());
		if(this->getMatrix() != NULL)
			glMultMatrixf(this->getMatrix());
		glGetFloatv(GL_MODELVIEW_MATRIX, this->matrix);
		glPopMatrix();
	}
	free(this->getAnimation());
	this->setAnimation(anim);
}

SceneComposite::SceneComposite(float* matrix, string id) {
	this->matrix = matrix;
	this->id = id;
}