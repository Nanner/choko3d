#include "SceneVertex.h"

unsigned int SceneVertex::currentDisplayList = 0;

SceneVertex::SceneVertex(): nodeVisited(false), childVisited(false), usesDisplayList(false), animation(NULL), initialMatrix(NULL) {}

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
    controlPoints.push_back(origin[1] + MOVE_HEIGHT);
    controlPoints.push_back(origin[2]);
    
	controlPoints.push_back(destination[0]);
    controlPoints.push_back(origin[1] + MOVE_HEIGHT);
    controlPoints.push_back(destination[2]);
    
	controlPoints.push_back(destination[0]);
    controlPoints.push_back(destination[1]);
    controlPoints.push_back(destination[2]);

	PieceAnimation* anim = new PieceAnimation(MOVE_DURATION, controlPoints);
	pieceAnimations.push(anim);
	PieceAnimation::addPieceAnimation(anim);
}

void SceneVertex::applyPieceAnimation() {
	if(pieceAnimations.empty())
		return;

	PieceAnimation* animation = pieceAnimations.front();
	if(animation->ended) {
		glPushMatrix();
		glLoadIdentity();
		glMultMatrixf(animation->getMatrix());
		if(this->getMatrix() != NULL) {
			float* currentMatrix = this->getMatrix();
			float* oldMatrix = new float[16];

			//This adds the previous position to the position history
			for(unsigned int i = 0; i < 16; i++) {
				oldMatrix[i] = currentMatrix[i];
			}
			positionHistory.push(oldMatrix);

			glMultMatrixf(currentMatrix);
		}
		else
			positionHistory.push(NULL);

		glGetFloatv(GL_MODELVIEW_MATRIX, this->matrix);
		glPopMatrix();
		pieceAnimations.pop();
		if(pieceAnimations.empty())
			return;
	}

	PieceAnimation* newAnimation = pieceAnimations.front();
	if(newAnimation != NULL)
		glMultMatrixf(newAnimation->getMatrix());
}

void SceneVertex::restartPiece() {
	if(initialMatrix != NULL) {
		for(unsigned int i = 0; i < 16; i++) {
			this->matrix[i] = this->initialMatrix[i]; 
		}
	}

	while(!pieceAnimations.empty())
		pieceAnimations.pop();
}

SceneVertex::~SceneVertex() {
	delete(matrix);
	delete(animation);
}

SceneComposite::SceneComposite(float* matrix, string id) {
	this->initialMatrix = new float[16];
	this->matrix = matrix;
	if(matrix != NULL) {
		for(unsigned int i = 0; i < 16; i++) {
			this->initialMatrix[i] = this->matrix[i]; 
		}
	}

	this->id = id;
}

void SceneVertex::undoMovement() {
	if(positionHistory.empty())
		return;

	float* oldMatrix = positionHistory.top();
	if(oldMatrix != NULL) {
		for(unsigned int i = 0; i < 16; i++)
			this->matrix[i] = oldMatrix[i];
	}
	else
		this->matrix = NULL;

	positionHistory.pop();
}