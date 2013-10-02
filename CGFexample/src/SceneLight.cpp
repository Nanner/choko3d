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

    glLightf(id, GL_SPOT_CUTOFF, angle);// set cutoff angle
    glLightf(id, GL_SPOT_EXPONENT, exponent); // set focusing strength
}