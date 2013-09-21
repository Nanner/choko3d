#ifndef YAFLIGHT_H
#define YAFLIGHT_H

#include "YAFElement.h"

class YAFLight: public YAFElement {
private:

public:
	string id;
	bool enabled;
	bool isOmni;

	//if omni light:
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

	//if spot light:
	float angle;
	float exponent;
	float directionX;
	float directionY;
	float directionZ;

	YAFLight(string id, bool enabled, bool isOmni, vector<int> values);
	void validate();
};

#endif