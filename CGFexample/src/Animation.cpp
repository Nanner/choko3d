#include "Animation.h"

Animation::Animation() {
    this->matrix = new float[16];
    glPushMatrix();
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();
}

void Animation::init(unsigned long t) {
    this->startTime = t;
    this->doReset = false;
}

float * Animation::getMatrix(){
    return matrix;
}