#include "Animation.h"

Animation::Animation() {
    this->matrix = new float[16];
    glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();
	isInitialized = false;
}

void Animation::init(unsigned long t) {
    this->startTime = t;
    this->doReset = false;
	this->isInitialized = true;
}

float * Animation::getMatrix(){
    return matrix;
}