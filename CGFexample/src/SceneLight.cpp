#include "SceneLight.h"

int SceneLight::lastLight = 0;
bool SceneLight::localLight = false;
bool SceneLight::lightEnabled = true;
bool SceneLight::doubleSided = false;
float SceneLight::ambient[4] = {1.0, 1.0, 1.0, 1.0};

int SceneLight::getGLLight(int number) {

    int lights[] = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};
	if ( number > 7 ) {
        return 0;
    } else {
        return lights[number];
    }
}

void SceneLight::resetLights() {
	lastLight = 0;
	localLight = false;
	lightEnabled = true;
	doubleSided = false;
	ambient[0] = 1.0; ambient[1] = 1.0; ambient[2] = 1.0; ambient[3] = 1.0;
}

void SceneLight::resetLastLight() {
    lastLight = 0;
}

SceneLight::SceneLight(bool enabled, string idString, float* pos,
					   float ambientR, float ambientG, float ambientB, float ambientA,
					   float diffuseR, float diffuseG, float diffuseB, float diffuseA,
					   float specularR, float specularG, float specularB, float specularA,
					   float *dir): CGFlight(getGLLight(lastLight), pos, dir), idString(idString) {
	lastLight++;

	if(enabled)
		enable();
	else
		disable();

	float ambient[4] = {ambientR, ambientG, ambientB, ambientA};
	setAmbient(ambient);

	float diffuse[4] = {diffuseR, diffuseG, diffuseB, diffuseA};
	setDiffuse(diffuse);

	float specular[4] = {specularR, specularG, specularB, specularA};
	setSpecular(specular);
}

bool SceneLight::isEnabled() const {
	return CGFlight::enabled;
}

string SceneLight::getIdString() const {
	return idString;
}

int SceneLight::getIdNumber() const {
	return id;
}

void SceneLight::enable() {
    CGFlight::enable();
    setAmbient(CGFlight::ambient);
    setDiffuse(CGFlight::diffuse);
    setSpecular(CGFlight::specular);
}

void SceneLight::update() {
    CGFlight::update();
    setAmbient(CGFlight::ambient);
    setDiffuse(CGFlight::diffuse);
    setSpecular(CGFlight::specular);
    glLightf(id, GL_SPOT_CUTOFF, 180.0); // reset
    glLightf(id, GL_SPOT_EXPONENT, 0.0); // reset
}

void SceneLight::draw() {
	update();
    
	material->apply();
}

SpotLight::SpotLight(bool enabled, string idString, float* pos, float *dir,
		  float ambientR, float ambientG, float ambientB, float ambientA,
		  float diffuseR, float diffuseG, float diffuseB, float diffuseA,
		  float specularR, float specularG, float specularB, float specularA,
		  float angle, float exponent): SceneLight(enabled, idString, pos,
		  ambientR, ambientG, ambientB, ambientA,
		  diffuseR, diffuseG, diffuseB, diffuseA,
		  specularR, specularG, specularB, specularA,
		  dir) {
    this->exponent = exponent;
	setAngle(angle);
    this->update();    
}


void SpotLight::update(){
    CGFlight::update();
    setAmbient(CGFlight::ambient);
    setDiffuse(CGFlight::diffuse);
    setSpecular(CGFlight::specular);
    glLightf(id, GL_SPOT_CUTOFF, angle);// set cutoff angle
    glLightf(id, GL_SPOT_EXPONENT, exponent); // set focusing strength
}

void SpotLight::draw() {
    update();
    
    material->apply();
}