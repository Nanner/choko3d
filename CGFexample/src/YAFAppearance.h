#ifndef YAFAPPEARANCE_H
#define YAFAPPEARANCE_H

#include "YAFElement.h"
#include "YAFTexture.h"
#include <map>

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
	string textureID;
	float texlength_s;
	float texlength_t;

	YAFAppearance(string id, vector<float> emissiveValues, vector<float> ambientValues, vector<float> diffuseValues, vector<float> specularValues, float shininess);
	YAFAppearance(string id, vector<float> emissiveValues, vector<float> ambientValues, vector<float> diffuseValues, vector<float> specularValues, float shininess, string textureID, float texlength_s, float texlength_t, map<string, YAFTexture> loadedTextures);

private:
	void initiateValues(string id, vector<float> emissiveValues, vector<float> ambientValues, vector<float> diffuseValues, vector<float> specularValues, float shininess);
	void validate();
	void validate(string textureID, map<string, YAFTexture> loadedTextures);
};

#endif