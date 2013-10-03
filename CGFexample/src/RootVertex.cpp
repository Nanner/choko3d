#include "RootVertex.h"

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
			Orthographic* newOrtho = new Orthographic(cam.id, cam.near, cam.far, cam.left, cam.right, cam.top, cam.bottom);
			this->cameras.insert(pair<string, CameraView*>(newOrtho->id, newOrtho));
		}
		else {
			Perspective* newPerspective = new Perspective(cam.id, cam.near, cam.far, cam.angle, cam.posX, cam.posY, cam.posZ, cam.targetX, cam.targetY, cam.targetZ);
			this->cameras.insert(pair<string, CameraView*>(newPerspective->id, newPerspective));
		}
	}
    
	map<string, YAFLight>::iterator it2 = lights.begin();
	for(; it2 != lights.end(); it2++) {
		YAFLight light = it2->second;
		if(light.isOmni && light.enabled) {
			float pos[4] = {light.locationX, light.locationY, light.locationZ, 1.0};
            
			SceneLight* newOmni = new SceneLight(light.enabled, light.id, pos,
                                                 light.ambientR, light.ambientG, light.ambientB, light.ambientA,
                                                 light.diffuseR, light.diffuseG, light.diffuseB, light.diffuseA,
                                                 light.specularR, light.specularG, light.specularB, light.specularA);
            
			this->lights.insert(pair<string, SceneLight*>(newOmni->getIdString(), newOmni));
		}
		else if (light.enabled) {
			float pos[4] = {light.locationX, light.locationY, light.locationZ, 1.0};
			float dir[3] = {light.directionX, light.directionY, light.directionZ};
            
			SpotLight* newSpot = new SpotLight(light.enabled, light.id, pos, dir,
                                               light.ambientR, light.ambientG, light.ambientB, light.ambientA,
                                               light.diffuseR, light.diffuseG, light.diffuseB, light.diffuseA,
                                               light.specularR, light.specularG, light.specularB, light.specularA,
                                               light.angle, light.exponent);
            
			this->lights.insert(pair<string, SceneLight*>(newSpot->getIdString(), newSpot));
		}
	}
}

void RootVertex::loadDefaultAppearance() {
	float ambient[4] = {1.0, 1.0, 1.0, 1.0};
	float diffuse[4] = {1.0, 1.0, 1.0, 1.0};
	float specular[4] = {0.0, 0.0, 0.0, 1.0};
	float shininess = 0.0;
    
	defaultAppearance = new CGFappearance(ambient, diffuse, specular, shininess);
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
