#ifndef CAMERABLOCK_H
#define CAMERABLOCK_H

#include "YAFElement.h"
#include "CameraChild.h"



class Camera: public YAFElement {
public:
	static string initialCameraID;

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

	Camera(string id, vector<float> nfaValues, vector<float> position, vector<float> target);
	Camera(string id, vector<float> values);
	void validate();
};

#endif