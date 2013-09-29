#ifndef _SCENELIGHT_H
#define _SCENELIGHT_H

#include <string>
#include "CGFlight.h"

using namespace std;

class SceneLight: public CGFlight {
	static int lastLight; //controls which GL_LIGHT* number we can attribute to the newly created light source
	string idString;
	
public:
	static bool localLight;
	static bool lightEnabled;
	static bool doubleSided;
	static float ambient[4];
	static int getGLLight(int number);

	SceneLight(bool enabled, string idString, float* pos,
	float ambientR, float ambientG, float ambientB, float ambientA,
	float diffuseR, float diffuseG, float diffuseB, float diffuseA,
	float specularR, float specularG, float specularB, float specularA,
	float *dir=NULL);

	bool isEnabled() const;
	string getIdString() const;
	int getIdNumber() const;
};

class SpotLight: public SceneLight {
public:
	SpotLight(bool enabled, string idString, float* pos, float *dir,
		float ambientR, float ambientG, float ambientB, float ambientA,
		float diffuseR, float diffuseG, float diffuseB, float diffuseA,
		float specularR, float specularG, float specularB, float specularA,
		float angle, float exponent);
};

#endif