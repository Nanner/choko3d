#include "CurvedAnimation.h"

CurvedAnimation::CurvedAnimation(float span, vector<float> controlPoints) : Animation() {
	this->totalSpan = span * 1000;
	this->controlPoints = controlPoints;
	P0.push_back(controlPoints.at(0)); P0.push_back(controlPoints.at(1)) ; P0.push_back(controlPoints.at(2));
	P1.push_back(controlPoints.at(3)); P1.push_back(controlPoints.at(4)) ; P1.push_back(controlPoints.at(5));
	P2.push_back(controlPoints.at(6)); P2.push_back(controlPoints.at(7)) ; P2.push_back(controlPoints.at(8));
	P3.push_back(controlPoints.at(9)); P3.push_back(controlPoints.at(10)) ; P3.push_back(controlPoints.at(11));
}

void CurvedAnimation::update(unsigned long time) {
    
    if(ended && loop) {
		reset();
		return;
	}
	else if(ended) {
		this->paused = true;
		this->pausedTime = 0;
		this->pauseStartTime = 0;
		return;
	}
    
	float curTime = time - this->startTime - pausedTime;
	float t = curTime / this->totalSpan;
    
    if (t > 1)
        ended = true;

	float x, y, z;

    // (1 - t)^3 P0 + 3(1 - t)^2*t*P1 + 3(1 - t)^t*2*P2 + t^3*P3

	x = (1-t)*(1-t)*(1-t) * P0.at(0) + 3*(1-t)*(1-t)*t* P1.at(0) + 3*(1-t)*t*t * P2.at(0) + t*t*t * P3.at(0);
	y = (1-t)*(1-t)*(1-t) * P0.at(1) + 3*(1-t)*(1-t)*t* P1.at(1) + 3*(1-t)*t*t * P2.at(1) + t*t*t * P3.at(1);
	z = (1-t)*(1-t)*(1-t) * P0.at(2) + 3*(1-t)*(1-t)*t* P1.at(2) + 3*(1-t)*t*t * P2.at(2) + t*t*t * P3.at(2);

	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(x, y, z);

	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();

}