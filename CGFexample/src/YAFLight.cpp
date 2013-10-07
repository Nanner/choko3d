#include "YAFLight.h"

YAFLight::YAFLight() {}

YAFLight::YAFLight(string id, bool enabled, vector<float> location, vector<float> ambient, vector<float> diffuse, vector<float> specular) {
    this->id = id;
    this->enabled = enabled;
    this->isOmni = true;
    
    locationX = location.at(0);
    locationY = location.at(1);
    locationZ = location.at(2);
    
    ambientR = ambient.at(0);
    ambientG = ambient.at(1);
    ambientB = ambient.at(2);
    ambientA = ambient.at(3);

    diffuseR = diffuse.at(0);
    diffuseG = diffuse.at(1);
    diffuseB = diffuse.at(2);
    diffuseA = diffuse.at(3);
    
    specularR = specular.at(0);
    specularG = specular.at(1);
    specularB = specular.at(2);
    specularA = specular.at(3);
    
    validate();
}

YAFLight::YAFLight(string id, bool enabled, vector<float> location, vector<float> ambient, vector<float> diffuse, vector<float> specular, float angle, float exponent, vector<float> direction){
    
    this->id = id;
    this->enabled = enabled;
    this->isOmni = false;
    
    locationX = location.at(0);
    locationY = location.at(1);
    locationZ = location.at(2);
    
    ambientR = ambient.at(0);
    ambientG = ambient.at(1);
    ambientB = ambient.at(2);
    ambientA = ambient.at(3);
    
    diffuseR = diffuse.at(0);
    diffuseG = diffuse.at(1);
    diffuseB = diffuse.at(2);
    diffuseA = diffuse.at(3);
    
    specularR = specular.at(0);
    specularG = specular.at(1);
    specularB = specular.at(2);
    specularA = specular.at(3);
    
    this->angle = angle;
    this->exponent = exponent;
    
    directionX = direction.at(0);
    directionY = direction.at(1);
    directionZ = direction.at(2);
    
    validate();
}

void YAFLight::validate() {
    char errorMessage[ERROR_LEN];
    
    if ( !validXYZ(locationX, locationY, locationZ)) {
        sprintf(errorMessage, "Light '%s': location (XYZ) error", id.c_str());
        throw(InvalidAttributeValueException(errorMessage));
    }
    
    if( !validRGBA(ambientR, ambientG, ambientB, ambientA) ) {
        sprintf(errorMessage, "Light '%s': ambient (RGBA) error", id.c_str());
		throw(InvalidAttributeValueException(errorMessage));
    }
    
    if( !validRGBA(diffuseR, diffuseG, diffuseB, diffuseA) ) {
        sprintf(errorMessage, "Light '%s': diffuse (RGBA) error", id.c_str());
		throw(InvalidAttributeValueException(errorMessage));
    }
    
    if( !validRGBA(specularR, specularG, specularB, specularA) ) {
        sprintf(errorMessage, "Light '%s': specular (RGBA) error", id.c_str());
		throw(InvalidAttributeValueException(errorMessage));
    }
    
    
    if ( !isOmni ){
        
        if ( ! validAngle(angle) ) {
            sprintf(errorMessage, "Omni light '%s': angle error", id.c_str());
            throw(InvalidAttributeValueException(errorMessage));
        }
        
        if ( !validXYZ(directionX, directionY, directionZ)){
            sprintf(errorMessage, "Omni light '%s': direction (XYZ) error", id.c_str());
            throw(InvalidAttributeValueException(errorMessage));
        }
        
    }

}

