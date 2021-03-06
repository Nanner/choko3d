#include "RootVertex.h"

unsigned int RootVertex::defaultAppearanceID = 0;

RootVertex::RootVertex(float* matrix,
                       string id,
                       YAFGlobal globals,
                       map<string, YAFCamera> cameras,
                       map<string, YAFLight> lights)
{
	loadDefaultAppearance();
	this->matrix = matrix;
	this->id = id;
	this->globals = globals;
    
	map<string, YAFCamera>::iterator it = cameras.begin();
	for(; it != cameras.end(); it++) {
		YAFCamera cam = it->second;
		if(cam.isOrtho) {
			Orthographic* newOrtho = new Orthographic(cam.id, cam.camNear, cam.camFar, cam.left, cam.right, cam.top, cam.bottom);
			this->cameras.insert(pair<string, CameraView*>(newOrtho->id, newOrtho));
		}
		else {
			Perspective* newPerspective = new Perspective(cam.id, cam.camNear, cam.camFar, cam.angle, cam.posX, cam.posY, cam.posZ, cam.targetX, cam.targetY, cam.targetZ);
			this->cameras.insert(pair<string, CameraView*>(newPerspective->id, newPerspective));
		}
	}
    
	map<string, YAFLight>::iterator it2 = lights.begin();
	for(; it2 != lights.end(); it2++) {
		YAFLight light = it2->second;
		if(light.isOmni) {
			float pos[4] = {light.locationX, light.locationY, light.locationZ, 1.0};
            
			SceneLight* newOmni = new SceneLight(light.enabled, light.id, pos,
                                                 light.ambientR, light.ambientG, light.ambientB, light.ambientA,
                                                 light.diffuseR, light.diffuseG, light.diffuseB, light.diffuseA,
                                                 light.specularR, light.specularG, light.specularB, light.specularA);
            
			this->lights.insert(pair<string, SceneLight*>(newOmni->getIdString(), newOmni));
			this->lightOnControls.insert(pair<string, int>(newOmni->getIdString(), newOmni->isEnabled()?1:0));
		}
		else {
			float pos[4] = {light.locationX, light.locationY, light.locationZ, 1.0};
			float dir[3] = {light.directionX, light.directionY, light.directionZ};
            
			SpotLight* newSpot = new SpotLight(light.enabled, light.id, pos, dir,
                                               light.ambientR, light.ambientG, light.ambientB, light.ambientA,
                                               light.diffuseR, light.diffuseG, light.diffuseB, light.diffuseA,
                                               light.specularR, light.specularG, light.specularB, light.specularA,
                                               light.angle, light.exponent);
            
			this->lights.insert(pair<string, SceneLight*>(newSpot->getIdString(), newSpot));
			this->lightOnControls.insert(pair<string, int>(newSpot->getIdString(), newSpot->isEnabled()?1:0));
		}
	}
}

void RootVertex::loadDefaultAppearance() {
	float ambient[4] = {1.0, 1.0, 1.0, 1.0};
	float diffuse[4] = {1.0, 1.0, 1.0, 1.0};
	float specular[4] = {0.0, 0.0, 0.0, 1.0};
	float emissivity[4] = {0.0, 0.0, 0.0, 0.0};
	float shininess = 0.0;
    
	defaultAppearance = new Appearance(ambient, diffuse, specular, emissivity, shininess);
	defaultAppearanceID = defaultAppearance->id;
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

RootVertex::~RootVertex() {
	delete(defaultAppearance);
	defaultAppearanceID = 0;

	map<string, CameraView*>::iterator camIt = cameras.begin();
	for(; camIt != cameras.end(); camIt++) {
		delete(camIt->second);
	}

	map<string, SceneLight*>::iterator lightIt = lights.begin();
	for(; lightIt != lights.end(); lightIt++) {
		delete(lightIt->second);
	}
}