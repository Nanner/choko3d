#ifndef YAFLIGHT_H
#define YAFLIGHT_H

#include "YAFElement.h"

class YAFLight: public YAFElement {
private:

public:
	string id;
	bool enabled;
	bool isOmni;

	float locationX;
	float locationY;
	float locationZ;
	float ambientR;
	float ambientG;
	float ambientB;
	float ambientA;
	float diffuseR;
	float diffuseG;
	float diffuseB;
	float diffuseA;
	float specularR;
	float specularG;
	float specularB;
	float specularA;

	//if spot light, more attributes:
	float angle;
	float exponent;
	float directionX;
	float directionY;
	float directionZ;

    YAFLight();
	YAFLight(string id, bool enabled, vector<float> location, vector<float> ambient, vector<float> diffuse, vector<float> specular);
    YAFLight(string id, bool enabled, vector<float> location, vector<float> ambient, vector<float> diffuse, vector<float> specular, float angle, float exponent, vector<float> direction);
	void validate();
};

#endif