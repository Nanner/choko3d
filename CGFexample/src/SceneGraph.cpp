#include "SceneGraph.h"

SceneGraph::SceneGraph(YAFReader* yafFile) {

	//Process the root node first
	string rootID = YAFNode::rootID;
	processRootNode(yafFile->nodes.find(rootID)->second, yafFile);

	//Process the rest of the nodes
	map<string, YAFNode>::iterator it = yafFile->nodes.begin();
	for(; it != yafFile->nodes.end(); it++) {
		if(it->first != rootID)
			processYAFNode(it->second);
	}

	//Process the links between nodes
	it = yafFile->nodes.begin();
	for(; it != yafFile->nodes.end(); it++) {
		processYAFNodeReferences(it->second);
	}
}


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


void SceneGraph::render() {
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
			render(*it);
			glPopMatrix();
		}
}

void SceneGraph::render(SceneVertex *v) {
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
			render(it->dest);
			glPopMatrix();
		}
	}

	it = (v->adj).begin();
	ite = (v->adj).end();
	for (; it !=ite; it++) {
		it->dest->childVisited = false;
	}
}

void SceneGraph::processRootNode(YAFNode root, YAFReader* yafFile) {
	RootVertex* newRoot = new RootVertex(root.transformationMatrix, root.id, yafFile->globals);
	rootVertex = newRoot;

	addVertex(newRoot);

	loadVertexPrimitives(root.primitives, newRoot);
}

void SceneGraph::processYAFNode(YAFNode yafNode) {
	//TODO check that shady thing about repeated primitives

	SceneComposite* newVertex = new SceneComposite(yafNode.transformationMatrix, yafNode.id);
	addVertex(newVertex);

	loadVertexPrimitives(yafNode.primitives, newVertex);
}

void SceneGraph::loadVertexPrimitives(vector<ScenePrimitive*> primitives, SceneVertex* vertex) {
	for(unsigned int i = 0; i < primitives.size(); i++) {
		ScenePrimitive* primitive = primitives.at(i);
		addVertex(primitive);
		addEdge(vertex, primitive);
	}
}

void SceneGraph::processYAFNodeReferences(YAFNode yafNode) {
	vector<SceneVertex* >::iterator vertexIterator = vertexSet.begin();

	//Find the vertex corresponding to this yafNode
	SceneVertex* vertex;

	for(; vertexIterator != vertexSet.end(); vertexIterator++) {
		if((*vertexIterator)->id == yafNode.id) {
			vertex = (*vertexIterator);
			break;
		}
	}

	//Add corresponding edges
	vector<string>::iterator idIterator = yafNode.nodeReferences.begin();
	for(; idIterator != yafNode.nodeReferences.end(); idIterator++) {
		for(vertexIterator = vertexSet.begin(); vertexIterator != vertexSet.end(); vertexIterator++) {
			if((*vertexIterator)->id == (*idIterator)) {
				addEdge(vertex, (*vertexIterator));
			}
		}
	}
}

void SceneGraph::configureScene() {
	rootVertex->setGlobals();
}
