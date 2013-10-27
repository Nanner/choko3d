#include "LinearAnimation.h"

LinearAnimation::LinearAnimation(float span, vector<float> controlPoints): Animation(){
    this->span = span;
    this->controlPoints = controlPoints;
}

void LinearAnimation::update(unsigned long t) {
    glPushMatrix();
    glLoadIdentity();
    
    // TODO apply transformations and store in the matrix
    
    float distance = t * 0.0001;
    
    //glRotated(t, 1.0, 0.0, 0.0);
    
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();
}