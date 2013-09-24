#include "SceneGraph.h"


SceneGraph::~SceneGraph() {
	for (unsigned int i = 0; i < vertexSet.size(); ++i) {
		delete vertexSet.at(i);
	}
}

bool SceneGraph::addVertex(SceneVertex *in) {
	//TODO check if vertex exists?
	/*vector<SceneVertex*>::iterator it= vertexSet.begin();
	vector<SceneVertex*>::iterator ite= vertexSet.end();
	for (; it!=ite; it++)
		if ( (*(*it)) == in ) return false;*/
	vertexSet.push_back(in);
	return true;
}

bool SceneGraph::addEdge(SceneVertex *sourc, SceneVertex *dest) {
	vector<SceneVertex*>::iterator it= vertexSet.begin();
	vector<SceneVertex*>::iterator ite= vertexSet.end();
	int found=0;
	//TODO verificar se edge já existe?
	/*SceneVertex *vS, *vD;
	while (found!=2 && it!=ite ) {
		if ( (*(*it)->info) == sourc )
		{ vS=*it; found++;}
		if ( (*(*it)->info) == dest )
		{ vD=*it; found++;}
		it ++;
	}
	if (found!=2) return false;
	vD->indegree++;
	vS->addEdge(vD,w);*/

	sourc->addEdge(dest);

	return true;
}


void SceneGraph::dfsVisit() {
	vector<SceneVertex *>::const_iterator it= vertexSet.begin();
	vector<SceneVertex *>::const_iterator ite= vertexSet.end();
	for (; it !=ite; it++)
		(*it)->nodeVisited=false;
	it=vertexSet.begin();
	for (; it !=ite; it++)
		if ( (*it)->nodeVisited==false ) {
			//glLoadIdentity();
			glPushMatrix();
			float* matrix = (*it)->getMatrix();
			if(matrix != NULL)
				glMultMatrixf(matrix);
			(*it)->render();
			dfsVisit(*it);
			glPopMatrix();
		}
}

void SceneGraph::dfsVisit(SceneVertex *v) {
	v->nodeVisited = true;
	v->childVisited = true;
	vector<SceneEdge>::iterator it= (v->adj).begin();
	vector<SceneEdge>::iterator ite= (v->adj).end();
	for (; it !=ite; it++) {
		if ( it->dest->childVisited == false ){
			glPushMatrix();
			float* matrix = it->dest->getMatrix();
			if(matrix != NULL)
				glMultMatrixf(matrix);
			it->dest->render();
			dfsVisit(it->dest);
			glPopMatrix();
		}
	}

	it = (v->adj).begin();
	ite = (v->adj).end();
	for (; it !=ite; it++) {
		it->dest->childVisited = false;
	}
}