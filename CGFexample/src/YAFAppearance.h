#ifndef YAFAPPEARANCE_H
#define YAFAPPEARANCE_H

#include "YAFElement.h"

class YAFAppearance: public YAFElement {
private:

public:
	string id;
	float emissiveR;
	float emissiveG;
	float emissiveB;
	float emissiveA;
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
	float shininess;

	bool usesTexture;
	string textureID; //textureref?
	float texlength_s;
	float texlength_t;

	YAFAppearance(string id, vector<float> values);
	YAFAppearance(string id, vector<float> values, string textureID, float texlength_s, float texlength_t);
	void validate();
};

#endif