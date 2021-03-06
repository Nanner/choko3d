#include "Animation.h"

Animation::Animation() {
    this->matrix = new float[16];
    glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();
	isInitialized = false;
	this->paused = false;
	this->loop = false;
	this->pausedTime = 0;
	this->ended = false;
}

void Animation::init(unsigned long t) {
    this->startTime = t;
	this->isInitialized = true;
	this->pausedTime = 0;
    this->ended = false;
}

float * Animation::getMatrix(){
    return matrix;
}

void Animation::update(unsigned long t) {
	if(paused == true) {
		//If pauseStartTime = 0, initialize value with current time
		if(pauseStartTime == 0)
			pauseStartTime = t;
		else
			pausedTime = t - pauseStartTime;
	}
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

Animation::~Animation() {
	delete(matrix);
}