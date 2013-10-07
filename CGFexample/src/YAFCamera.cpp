#include "YAFCamera.h"

string YAFCamera::initialCameraID = "";

YAFCamera::YAFCamera(string id, vector<float> nfaValues, vector<float> position, vector<float> target): isOrtho(false) {

	this->id = id;
    
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
    char errorMessage[ERROR_LEN];
    
    if ( ! validMinMax(near, far) ) {
        sprintf(errorMessage, "Camera '%s': min/max error", id.c_str());
		throw(InvalidAttributeValueException(errorMessage));
    }
    
	if(isOrtho) {
        
        if ( ! validMinMax(left, right) ) {
            sprintf(errorMessage, "Camera '%s': left/right error", id.c_str());
            throw(InvalidAttributeValueException(errorMessage));
        }
        
        if ( ! validMinMax(top, bottom)) {
            sprintf(errorMessage, "Camera '%s': top/bottom error", id.c_str());
            throw(InvalidAttributeValueException(errorMessage));
        }
	}
	else {
		if( !validAngle(angle) ) {
            sprintf(errorMessage, "Camera '%s': angle error", id.c_str());
			throw(InvalidAttributeValueException(errorMessage));
        }
        
        if ( !validXYZ(posX, posY, posZ) ){
            sprintf(errorMessage, "Camera '%s': XYZ error", id.c_str());
            throw(InvalidAttributeValueException(errorMessage));
        }
        
        if ( !validXYZ(targetX, targetY, targetZ) ) {
            sprintf(errorMessage, "Camera '%s': target XYZ error", id.c_str());
            throw(InvalidAttributeValueException(errorMessage));

        }
	}

}