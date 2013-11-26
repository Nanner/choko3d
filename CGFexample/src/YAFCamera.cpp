#include "YAFCamera.h"

string YAFCamera::initialCameraID = "";

YAFCamera::YAFCamera(string id, vector<float> nfaValues, vector<float> position, vector<float> target): isOrtho(false) {

	this->id = id;
    
	camNear = nfaValues.at(0);
	camFar = nfaValues.at(1);
	angle = nfaValues.at(2);

	posX = position.at(0);
	posY = position.at(1);
	posZ = position.at(2);

	targetX = target.at(0);
	targetY = target.at(1);
	targetZ = target.at(2);

	validate();

}

YAFCamera::YAFCamera(string id, vector<float> values): isOrtho(true) {

	this->id = id;

	camNear = values.at(0);
	camFar = values.at(1);
	left = values.at(2);
	right = values.at(3);
	top = values.at(4);
	bottom = values.at(5);

	validate();

}

void YAFCamera::validate() {}