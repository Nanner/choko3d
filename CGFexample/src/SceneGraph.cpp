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

	findShaders();
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

	glPushMatrix();
		glTranslatef(13.5, 8.625, 17.5);

		glRasterPos3f(0, 0, 0);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'A');

		glRasterPos3f(0, 0, 5);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'B');

		glRasterPos3f(3, 0, 3);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'C');

		glRasterPos3f(6, 1, 6);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'D');
	glPopMatrix();
    
	vector<SceneVertex *>::const_iterator it= vertexSet.begin();
	vector<SceneVertex *>::const_iterator ite= vertexSet.end();
	for (; it !=ite; it++)
		(*it)->nodeVisited=false;
	
    // TODO display lists still cause errors when used in the root vertex
    // some transformations seem to be reseting

	Appearance* appearance = rootVertex->getAppearance();

	if(appearance == NULL)
		appearance = rootVertex->defaultAppearance;

	if ( rootVertex->usesDisplayList && rootVertex->initializedDisplayList(appearance->id) );
		//glCallList(rootVertex->getDisplayList(appearance->id));
	else {
		if (rootVertex->usesDisplayList && ! rootVertex->initializedDisplayList(appearance->id) && stackReady && displayListOrder.top() == rootVertex->getDisplayList(appearance->id))
			glNewList(rootVertex->getDisplayList(appearance->id), GL_COMPILE_AND_EXECUTE);
		else if(rootVertex->usesDisplayList && !rootVertex->initializedDisplayList(appearance->id) && !stackReady)
			displayListOrder.push(rootVertex->getDisplayList(appearance->id));

		glPushMatrix();
		rootVertex->defaultAppearance->apply();

		Appearance* app = rootVertex->getAppearance();
		if (app != NULL)
			app->apply();

		float* matrix = rootVertex->getMatrix();
		if(matrix != NULL)
			glMultMatrixf(matrix);

		render(rootVertex);

		if (rootVertex->usesDisplayList && ! rootVertex->initializedDisplayList(appearance->id) && stackReady && displayListOrder.top() == rootVertex->getDisplayList(appearance->id)){
			glEndList();
			rootVertex->initializeDisplayList(appearance->id);
			displayListOrder.pop();
			stackReady = false;
		}
	}

	glPopMatrix();

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
		if ( it->dest->childVisited == false ) {
			Appearance* appearance = it->dest->getAppearance();

			if(it->dest->inheritedAppearance)
				appearance = v->getAppearance();

			if(appearance == NULL)
				appearance = rootVertex->defaultAppearance;

			if(v->id == "leftWall")
				printf("Creating for leftWall\n");


			if ( it->dest->usesDisplayList && it->dest->initializedDisplayList(appearance->id) );
				//glCallList(it->dest->getDisplayList(appearance->id));
			else {
				if (it->dest->usesDisplayList && ! it->dest->initializedDisplayList(appearance->id) && stackReady && displayListOrder.top() == it->dest->getDisplayList(appearance->id))
					glNewList(it->dest->getDisplayList(appearance->id), GL_COMPILE_AND_EXECUTE);
				else if(it->dest->usesDisplayList && ! it->dest->initializedDisplayList(appearance->id) && !stackReady)
					displayListOrder.push(it->dest->getDisplayList(appearance->id));

				glPushMatrix();
				rootVertex->defaultAppearance->apply();

				if(it->dest->inheritedAppearance)
					it->dest->setAppearance(v->getAppearance());
				
				Appearance* app;
				app = it->dest->getAppearance();

				if (app != NULL)
					app->apply();
					

				// TODO fix applying animation matrix
				if(it->dest->getAnimation() != NULL){
					float * animationMatrix = it->dest->getAnimation()->getMatrix();
					glMultMatrixf(animationMatrix);
				}

				float* matrix = it->dest->getMatrix();
				if(matrix != NULL)
					glMultMatrixf(matrix);

				if(it->dest->getAnimation() != NULL)
					it->dest->getAnimation()->applyRotation();

				it->dest->draw();
				render(it->dest);

				glPopMatrix();

				if (it->dest->usesDisplayList && ! it->dest->initializedDisplayList(appearance->id) && stackReady && displayListOrder.top() == it->dest->getDisplayList(appearance->id)){
					glEndList();
					it->dest->initializeDisplayList(appearance->id);
					printf("Created displayList for %s, with appearance %u\n", it->dest->id.c_str(), appearance->id);
					displayListOrder.pop();
					stackReady = false;
				}
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

//This function searches the graph to see if any shaders are used, adding them to the shader vector
//And toggling the hasShader bool to true. This is needed for the demoscene update function
void SceneGraph::findShaders() {
	CGFshader* shader;

	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		shader = vertexSet.at(i)->getShader();
		if(shader != NULL) {
			printf("Found shader!\n");
			currentShaders.push_back(shader);
			hasShader = true;
		}
	}
}

//This function updates the shaders with a time t
void SceneGraph::updateShaders(unsigned long t) {
	if(!hasShader)
		return;

	for(unsigned int i = 0; i < currentShaders.size(); i++) {
		currentShaders.at(i)->bind();
		currentShaders.at(i)->update(t);
		currentShaders.at(i)->unbind();
	}
}