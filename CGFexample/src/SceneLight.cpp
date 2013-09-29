#include "SceneLight.h"

int SceneLight::lastLight = 0;
bool SceneLight::localLight = false;
bool SceneLight::lightEnabled = true;
bool SceneLight::doubleSided = false;
float SceneLight::ambient[4] = {1.0, 1.0, 1.0, 1.0};

int SceneLight::getGLLight(int number) {

	switch(number){
	case 0:
		return GL_LIGHT0;
		break;
	case 1:
		return GL_LIGHT1;
		break;
	case 2:
		return GL_LIGHT2;
		break;
	case 3:
		return GL_LIGHT3;
		break;
	case 4:
		return GL_LIGHT4;
		break;
	case 5:
		return GL_LIGHT5;
		break;
	case 6:
		return GL_LIGHT6;
		break;
	case 7:
		return GL_LIGHT7;
		break;
	default:
		return 0;
		break;
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
	
	setAngle(angle);
	glLightf(id, GL_SPOT_EXPONENT, exponent);
}