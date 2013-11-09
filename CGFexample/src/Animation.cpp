#include "Animation.h"

Animation::Animation() {
    this->matrix = new float[16];
    glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();
	isInitialized = false;
	this->loop = false;
	this->pausedTime = 0;
}

void Animation::init(unsigned long t) {
    this->startTime = t;
	this->isInitialized = true;
	this->pausedTime = 0;
}

float * Animation::getMatrix(){
    return matrix;
}

void Animation::reset() {
	//This forces init to be called upon next update
	this->isInitialized = false;
	if(!loop) {
		this->paused = true;
	}
		this->pausedTime = 0;
		this->pauseStartTime = 0;

	if(loop)
		resume();
}

void Animation::pause() {
	this->paused = true;
	this->startTime += pausedTime;
	this->pausedTime = 0;
	this->pauseStartTime = 0;
}

void Animation::resume() {
	this->paused = false;
}