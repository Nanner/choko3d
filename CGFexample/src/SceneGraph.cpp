#include "SceneGraph.h"

SceneGraph::SceneGraph(YAFReader* yafFile) {
	stackReady = false;

	SceneLight::localLight = yafFile->globalLighting.local;
	SceneLight::lightEnabled = yafFile->globalLighting.enabled;
	SceneLight::doubleSided = yafFile->globalLighting.doublesided;
	SceneLight::ambient[0] = yafFile->globalLighting.ambientR;
	SceneLight::ambient[1] = yafFile->globalLighting.ambientG;
	SceneLight::ambient[2] = yafFile->globalLighting.ambientB;
	SceneLight::ambient[3] = yafFile->globalLighting.ambientA;

    //read the textures and appearances
    map<string, YAFAppearance>::iterator appearanceItr = yafFile->appearances.begin();
    for(; appearanceItr != yafFile->appearances.end(); appearanceItr++) {
        YAFAppearance a = appearanceItr->second;
		float amb[4]  = {a.ambientR,  a.ambientG,  a.ambientB,  a.ambientA };
        float dif[4]  = {a.diffuseR,  a.diffuseG,  a.diffuseB,  a.diffuseA };
        float spec[4] = {a.specularR, a.specularG, a.specularB, a.specularA};
		float emis[4] = {a.emissiveR, a.emissiveG, a.emissiveB, a.emissiveA};
        float shininess =  a.shininess;
        Appearance* appearance = new Appearance(amb, dif, spec, emis, shininess);
        
        if ( a.usesTexture ) {
            YAFTexture yafTexture = yafFile->textures.at(a.textureID);
            appearance->setTexture(yafTexture.file);
			appearance->setTextureWrap(GL_REPEAT, GL_REPEAT);
			appearance->setTexLength_s(a.texlength_s);
			appearance->setTexLength_t(a.texlength_t);
        }
        
        appearances.insert(pair<string, Appearance*>(appearanceItr->first, appearance));
	}
    
    this->animations = yafFile->animations;
    
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
    
	// initialize display lists
    // TODO check returned unsigned int from gen lists
    glGenLists(SceneVertex::currentDisplayList);
}


SceneGraph::~SceneGraph() {
	for (unsigned int i = 0; i < vertexSet.size(); ++i) {
		delete vertexSet.at(i);
	}
}

bool SceneGraph::addVertex(SceneVertex *in) {
	vertexSet.push_back(in);
	return true;
}

bool SceneGraph::addEdge(SceneVertex *sourc, SceneVertex *dest) {
	sourc->addEdge(dest);
	return true;
}





void SceneGraph::render() {
    
	vector<SceneVertex *>::const_iterator it= vertexSet.begin();
	vector<SceneVertex *>::const_iterator ite= vertexSet.end();
	for (; it !=ite; it++)
		(*it)->nodeVisited=false;
	
    // TODO display lists still cause errors when used in the root vertex
    // some transformations seem to be reseting
	if ( rootVertex->usesDisplayList && rootVertex->initializedDisplayList )
		glCallList(rootVertex->getDisplayList());
	else {
        
		if (rootVertex->usesDisplayList && ! rootVertex->initializedDisplayList && stackReady && displayListOrder.top() == rootVertex->getDisplayList())
			glNewList(rootVertex->getDisplayList(), GL_COMPILE_AND_EXECUTE);
		else if(rootVertex->usesDisplayList && !rootVertex->initializedDisplayList && !stackReady)
			displayListOrder.push(rootVertex->getDisplayList());
        
		glPushMatrix();
		rootVertex->defaultAppearance->apply();
        
		Appearance* appearance = rootVertex->getAppearance();
		if (appearance != NULL)
			appearance->apply();
        
		float* matrix = rootVertex->getMatrix();
		if(matrix != NULL)
			glMultMatrixf(matrix);
        
		render(rootVertex);
        
		glPopMatrix();
        
		if (rootVertex->usesDisplayList && ! rootVertex->initializedDisplayList && stackReady && displayListOrder.top() == rootVertex->getDisplayList()){
			glEndList();
			rootVertex->initializedDisplayList = true;
			displayListOrder.pop();
            stackReady = false;
		}
	}

	//Indicate that the graph was rendered the first time and so the displayList order stack is ready to be used
	if(!stackReady && !displayListOrder.empty())
		stackReady = true;
}

void SceneGraph::render(SceneVertex *v) {
    
	v->nodeVisited = true;
	v->childVisited = true;
    vector<SceneEdge>::iterator it, ite;
    
    // TODO display list inside display list doesn't work
    // extra verifications are still necessary
    
    it = (v->adj).begin();
    ite = (v->adj).end();
    for (; it !=ite; it++) {
        if ( it->dest->usesDisplayList && it->dest->initializedDisplayList )
            glCallList(it->dest->getDisplayList());
        else {
            if (it->dest->usesDisplayList && ! it->dest->initializedDisplayList && stackReady && displayListOrder.top() == it->dest->getDisplayList())
                glNewList(it->dest->getDisplayList(), GL_COMPILE_AND_EXECUTE);
			else if(it->dest->usesDisplayList && ! it->dest->initializedDisplayList && !stackReady)
				displayListOrder.push(it->dest->getDisplayList());
            
			if ( it->dest->childVisited == false ){
				glPushMatrix();
				rootVertex->defaultAppearance->apply();
                
				if(it->dest->inheritedAppearance)
					it->dest->setAppearance(v->getAppearance());
                
				Appearance* appearance = it->dest->getAppearance();
				if (appearance != NULL)
					appearance->apply();
                
                // TODO fix applying animation matrix
                if(it->dest->getAnimation() != NULL){
                    float * animationMatrix = it->dest->getAnimation()->getMatrix();
                    glMultMatrixf(animationMatrix);
                }
                
				float* matrix = it->dest->getMatrix();
				if(matrix != NULL)
					glMultMatrixf(matrix);
                
				it->dest->draw();
				render(it->dest);
                
				glPopMatrix();
			}

			if (it->dest->usesDisplayList && ! it->dest->initializedDisplayList && stackReady && displayListOrder.top() == it->dest->getDisplayList()){
				glEndList();
				it->dest->initializedDisplayList = true;
				displayListOrder.pop();
                stackReady = false;
			}
        }
        
    }
    
    
	it = (v->adj).begin();
	ite = (v->adj).end();
	for (; it !=ite; it++) {
		it->dest->childVisited = false;
	}
    
	//Restore appearance back to null
	if(v->inheritedAppearance)
		v->appearance = NULL;
    
	//Re-apply default appearance
	rootVertex->defaultAppearance->apply();
}

void SceneGraph::processRootNode(YAFNode root, YAFReader* yafFile) {
	RootVertex* newRoot = new RootVertex(root.transformationMatrix, root.id, yafFile->globals, yafFile->cameras, yafFile->lights);
	rootVertex = newRoot;

	if (root.appearanceID != "") {
		newRoot->setAppearance(appearances.at(root.appearanceID));
		newRoot->inheritedAppearance = false;
	}
	else {
		newRoot->setAppearance(NULL);
		newRoot->inheritedAppearance=false;
	}
    
    if (root.usesDisplayList)
        newRoot->activateDisplayList();

	addVertex(newRoot);

	loadVertexPrimitives(root.primitives, newRoot);
}

void SceneGraph::processYAFNode(YAFNode yafNode) {
	SceneComposite* newVertex = new SceneComposite(yafNode.transformationMatrix, yafNode.id);
    if ( yafNode.appearanceID != "") {
        newVertex->setAppearance(appearances.at(yafNode.appearanceID));
		newVertex->inheritedAppearance = false;
	}
	else {
		newVertex->setAppearance(NULL);
		newVertex->inheritedAppearance = true;
	}
    
    if (yafNode.usesDisplayList)
        newVertex->activateDisplayList();
    
    if (yafNode.animationID.compare("") != 0) {
        Animation * animation = animations.at(yafNode.animationID);
        newVertex->setAnimation(animation);
    }

	addVertex(newVertex);

	loadVertexPrimitives(yafNode.primitives, newVertex);
}

void SceneGraph::loadVertexPrimitives(vector<ScenePrimitive*> primitives, SceneVertex* vertex) {
	for(unsigned int i = 0; i < primitives.size(); i++) {
		ScenePrimitive* primitive = primitives.at(i);
        primitive->setAppearance(vertex->getAppearance());
		primitive->inheritedAppearance = true;
		addVertex(primitive);
		addEdge(vertex, primitive);
	}
}

void SceneGraph::processYAFNodeReferences(YAFNode yafNode) {
	vector<SceneVertex* >::iterator vertexIterator = vertexSet.begin();

	//Find the vertex corresponding to this yafNode
	SceneVertex* vertex = (*vertexIterator);

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

RootVertex* SceneGraph::getRootVertex() {
	return rootVertex;
}

void SceneGraph::configureScene() {
	rootVertex->setGlobals();

	// Enables lighting computations
	if(SceneLight::lightEnabled)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	// Sets up some lighting parameters
	if(SceneLight::doubleSided)
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	else
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, SceneLight::ambient);  // Define ambient light

	if(SceneLight::localLight)
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	else
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);

	glEnable(GL_NORMALIZE);
}

CGFcamera* SceneGraph::getInitialCamera() {
	return rootVertex->cameras.find(YAFCamera::initialCameraID)->second;
}

void SceneGraph::drawLights() {
	map<string, SceneLight*>::iterator it = rootVertex->lights.begin();
	for(; it != rootVertex->lights.end(); it++) {
		it->second->draw();
	}
}
