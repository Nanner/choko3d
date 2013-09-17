#ifndef CAMERABLOCK_H
#define CAMERABLOCK_H

#include "InfoBlock.h"

class CameraBlock: public InfoBlock {
public:
	string initialCameraID;
	vector<CameraAttributes> cameras;

	CameraBlock(string initialCameraID, vector<CameraAttributes> cameras);
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

#endif