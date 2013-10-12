#include "YAFGlobalLighting.h"

YAFGlobalLighting::YAFGlobalLighting(){}

YAFGlobalLighting::YAFGlobalLighting(vector<bool> ligthingProperties, vector<float> ambientRGBA){
    doublesided = ligthingProperties.at(0);
    local = ligthingProperties.at(1);
    enabled = ligthingProperties.at(2);
    
    ambientR = ambientRGBA.at(0);
    ambientG = ambientRGBA.at(1);
    ambientB = ambientRGBA.at(2);
    ambientA = ambientRGBA.at(3);
    
    this->validate();
}

void YAFGlobalLighting::validate() {
    
	if( !validRGBA(ambientR, ambientG, ambientB, ambientA) ) {
		throw(InvalidAttributeValueException("Global light: ambient (RGBA)"));
    }
}