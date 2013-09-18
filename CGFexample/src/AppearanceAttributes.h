#ifndef APPEARANCEATTRIBUTES_H
#define APPEARANCEATTRIBUTES_H

#include "InfoBlock.h"

class AppearanceAttributes: public InfoBlock {
private:
	static vector<string> usedAppearanceIDs;

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

	AppearanceAttributes(string id, vector<float> values);
	AppearanceAttributes(string id, vector<float> values, string textureID, float texlength_s, float texlength_t);
	void validate();
};

#endif