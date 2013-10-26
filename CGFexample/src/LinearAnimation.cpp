#include "LinearAnimation.h"

LinearAnimation::LinearAnimation(float span, vector<float> controlPoints){
    this->span = span;
    this->controlPoints = controlPoints;
    
    printf("im created!\n");
    for (int i = 0; i < controlPoints.size(); i += 3) {
        printf("x: %f, y: %f, z: %f\n", controlPoints.at(i), controlPoints.at(i+1), controlPoints.at(i+2) );
    }
}