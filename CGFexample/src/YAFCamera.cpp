#include "YAFCamera.h"

string YAFCamera::initialCameraID = "";

YAFCamera::YAFCamera(string id, vector<float> nfaValues, vector<float> position, vector<float> target): isOrtho(false) {

	this->id = id;
    
    if ( initialCameraID == "" ) initialCameraID = id;

	near = nfaValues.at(0);
	far = nfaValues.at(1);
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

	near = values.at(0);
	far = values.at(1);
	left = values.at(2);
	right = values.at(3);
	top = values.at(4);
	bottom = values.at(5);

	validate();

}

void YAFCamera::validate() {

	//TODO check the correct bounds for the defines
	if(near < XYZ_MIN || far > XYZ_MAX)
		throw(InvalidAttributeValueException("Camera: min/max"));

	if(isOrtho) {
	if(left < XYZ_MIN || right > XYZ_MAX)
		throw(InvalidAttributeValueException("Camera: left/right"));

	if(top < XYZ_MIN || bottom > XYZ_MAX)
		throw(InvalidAttributeValueException("Camera: top/bottom"));
	}
	else {
		if(angle < ANGLE_MIN || angle > ANGLE_MAX)
			throw(InvalidAttributeValueException("Camera: angle"));

		if(posX < XYZ_MIN || posX > XYZ_MAX)
			throw(InvalidAttributeValueException("Camera: X position"));
		if(posY < XYZ_MIN || posY > XYZ_MAX)
			throw(InvalidAttributeValueException("Camera: Y position"));
		if(posZ < XYZ_MIN || posY > XYZ_MAX)
			throw(InvalidAttributeValueException("Camera: Z position"));

		if(targetX < XYZ_MIN || targetX > XYZ_MAX)
			throw(InvalidAttributeValueException("Camera: Target's X"));
		if(targetY < XYZ_MIN || targetY > XYZ_MAX)
			throw(InvalidAttributeValueException("Camera: Target's Y"));
		if(targetZ < XYZ_MIN || targetZ > XYZ_MAX)
			throw(InvalidAttributeValueException("Camera: Target's Z"));

	}

}