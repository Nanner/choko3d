#include "SceneVertex.h"

SceneVertex::SceneVertex(): nodeVisited(false), childVisited(false) {}

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

RootVertex::RootVertex(float* matrix, string id, YAFGlobal globals, map<string, YAFCamera> cameras) {
	this->matrix = matrix;
	this->id = id;
	this->globals = globals;

	map<string, YAFCamera>::iterator it = cameras.begin();
	for(; it != cameras.end(); it++) {
		YAFCamera cam = it->second;
		if(cam.isOrtho) {
			Orthographic* newOrtho = new Orthographic(cam.id, cam.near, cam.far, cam.left, cam.right, cam.top, cam.bottom);
			this->cameras.insert(pair<string, CameraView*>(newOrtho->id, newOrtho));
		}
		else {
			Perspective* newPerspective = new Perspective(cam.id, cam.near, cam.far, cam.angle, cam.posX, cam.posY, cam.posZ, cam.targetX, cam.targetY, cam.targetZ);
			this->cameras.insert(pair<string, CameraView*>(newPerspective->id, newPerspective));
		}
	}
}

void RootVertex::setGlobals() {
	glClearColor(globals.red, globals.green, globals.blue, globals.alpha);
	glClear(GL_COLOR_BUFFER_BIT);

	string drawmode = globals.drawmode;
	if(drawmode == "fill")
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if(drawmode == "line")
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if(drawmode == "point")
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	string shading = globals.shading;
	if(shading == "gouraud")
		glShadeModel(GL_SMOOTH);
	else if(shading == "flat")
		glShadeModel(GL_FLAT);

	string cullface = globals.cullface;
	if(cullface == "none")
		glDisable(GL_CULL_FACE);
	else {
		glEnable(GL_CULL_FACE);
		if(cullface == "back")
			glCullFace(GL_BACK);
		else if(cullface == "front")
			glCullFace(GL_FRONT);
		else if(cullface == "both")
			glCullFace(GL_FRONT_AND_BACK);
	}

	string cullorder = globals.cullorder;
	if(cullorder == "CCW")
		glFrontFace(GL_CCW);
	else if(cullorder == "CW")
		glFrontFace(GL_CW);

}

void RootVertex::setInitialCamera() {
	cameras.find(YAFCamera::initialCameraID)->second->applyView();
}

SceneComposite::SceneComposite(float* matrix, string id) {
	this->matrix = matrix;
	this->id = id;
}