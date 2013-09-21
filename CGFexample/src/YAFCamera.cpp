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
    
	//TODO check the correct bounds for the defines
    if ( ! validMinMax(near, far) )
		throw(InvalidAttributeValueException("Camera: min/max"));
    
	if(isOrtho) {
        
        if ( ! validMinMax(left, right) )
            throw(InvalidAttributeValueException("Camera: left/right"));
        
        if ( ! validMinMax(top, bottom))
            throw(InvalidAttributeValueException("Camera: top/bottom"));
	}
	else {
		if( !validAngle(angle) )
			throw(InvalidAttributeValueException("Camera: angle"));
        
        if ( !validXYZ(posX, posY, posZ) ){
            throw(InvalidAttributeValueException("Camera: XYZ position"));
        }
        
        if ( !validXYZ(targetX, targetY, targetZ) ) {
            throw(InvalidAttributeValueException("Camera: Target's XYZ"));

        }
	}

}