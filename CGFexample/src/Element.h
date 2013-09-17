#ifdef ELEMENT_H
#define ELEMENT_H

#include <string>

using namespace std;

class InfoBlock {
public:
	virtual bool validate();
};

class GlobalAttributes: public InfoBlock {
public:
	float red;
	float green;
	float blue;
	float alpha;

	string drawmode;
	string shading;
	string cullface;
	string cullorder;

	GlobalAttributes(vector<float> rgbaValues, vector<string> modes); //that string vector might be changed
	bool validate();
};

class CameraAttributes: public InfoBlock {
private:
	static vector<string> usedCameraIDs;

public:
	bool isOrtho;
	string id;
	float near;
	float far;

	//If ortho camera:
	float left;
	float right;
	float top;
	float bottom;

	//If perspective camera:
	float angle;
	float posX;
	float posY;
	float posZ;
	float targetX;
	float targetY;
	float targetZ;

	CameraAttributes(bool orthoCamera, string ID, vector<int> values);
	bool validate();
};

class CameraBlock: public InfoBlock {
public:
	string initialCameraID;
	vector<CameraAttributes> cameras;

	CameraBlock(string initialCameraID, vector<CameraAttributes> cameras);
};

class LightAttributes: public InfoBlock {
private:
	static vector<string> usedLightIDs;

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

	LightAttributes(string id, bool enabled, bool isOmni, vector<int> values);
	bool validate();
};

class LightingBlock: public InfoBlock {
public:
	bool doublesided;
	bool local;
	bool enabled;
	float ambientR;
	float ambientG;
	float ambientB;
	float ambientA;
	vector<LightAttributes> lights;

	LigthingBlock(vector<bool> ligthingProperties, vector<float> ambientRGBA, vector<LightAttributes> lights);
};

class TextureAttributes: public InfoBlock {
private:
	static vector<string> usedTextureIDs;

public:
	string id;
	string file;

	TextureAttributes(string id, string file);
	bool validate();
};

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

	TextureAttributes(string id, vector<float> values);
	TextureAttributes(string id, vector<float> values, string textureID, float texlength_s, float texlength_t);
	bool validate();
};


#endif