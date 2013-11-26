#ifndef YAFCAMERA_H
#define YAFCAMERA_H

#include "YAFElement.h"

class YAFCamera: public YAFElement {
public:
	static string initialCameraID;

	bool isOrtho;
	string id;
	float camNear;
	float camFar;

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

	YAFCamera(string id, vector<float> nfaValues, vector<float> position, vector<float> target);
	YAFCamera(string id, vector<float> values);
	void validate();
};

#endif