#include "PieceAnimation.h"

queue<PieceAnimation*> PieceAnimation::globalPieceAnimations;

void PieceAnimation::updatePieceAnimations(unsigned long t) {
	if(globalPieceAnimations.empty())
		return;
	PieceAnimation* currentAnimation = globalPieceAnimations.front();
	if(currentAnimation->ended) {
		globalPieceAnimations.pop();
        delete(currentAnimation);
    }

	if(globalPieceAnimations.empty())
		return;

	currentAnimation = globalPieceAnimations.front();
	currentAnimation->resume();
	if (currentAnimation->isInitialized)
		currentAnimation->update(t);
	else
		currentAnimation->init(t);
}

void PieceAnimation::addPieceAnimation(PieceAnimation* pieceAnimation) {
	globalPieceAnimations.push(pieceAnimation);
}

bool PieceAnimation::pendingAnimations() {
	if(globalPieceAnimations.empty() || (globalPieceAnimations.size() == 1 && globalPieceAnimations.front()->ended)) {
		return false;
	}

	return true;
}

void PieceAnimation::clearGlobalPieceAnimations() {
	while(!globalPieceAnimations.empty()) {
		PieceAnimation* animation = globalPieceAnimations.front();
		globalPieceAnimations.pop();
		delete animation;
	}
}

PieceAnimation::PieceAnimation(float span, vector<float> controlPoints): Animation(){
    this->totalSpan = span * 1000;
    this->controlPoints = controlPoints;
    
    P0.push_back(0.0);
    P0.push_back(0.0);
    P0.push_back(0.0);
    
	P1.push_back(controlPoints.at(3) - controlPoints.at(0));
    P1.push_back(controlPoints.at(4) - controlPoints.at(1));
    P1.push_back(controlPoints.at(5) - controlPoints.at(2));
    
	P2.push_back(controlPoints.at(6) - controlPoints.at(3));
    P2.push_back(controlPoints.at(7) - controlPoints.at(1));
    P2.push_back(controlPoints.at(8) - controlPoints.at(5));
    
	P3.push_back( controlPoints.at(9) - controlPoints.at(0));
    P3.push_back(controlPoints.at(10) - controlPoints.at(1));
    P3.push_back(controlPoints.at(11) - controlPoints.at(2));
    
    float x, y, z;
    
    for (float t = 0.0; t < 1.0 - T_INTERVAL; t += T_INTERVAL) {
        x = (1-t)*(1-t)*(1-t) * P0.at(0) + 3*(1-t)*(1-t)*t* P1.at(0) + 3*(1-t)*t*t * P2.at(0) + t*t*t * P3.at(0);
        y = (1-t)*(1-t)*(1-t) * P0.at(1) + 3*(1-t)*(1-t)*t* P1.at(1) + 3*(1-t)*t*t * P2.at(1) + t*t*t * P3.at(1);
        z = (1-t)*(1-t)*(1-t) * P0.at(2) + 3*(1-t)*(1-t)*t* P1.at(2) + 3*(1-t)*t*t * P2.at(2) + t*t*t * P3.at(2);
        vector<float> point;
        point.push_back(x);
        point.push_back(y);
        point.push_back(z);
        timeSpans.push_back(point);
    }
    
    vector<float> lastPoint;
    lastPoint.push_back(P3.at(0));
    lastPoint.push_back(P3.at(1));
    lastPoint.push_back(P3.at(2));
    timeSpans.push_back(lastPoint);

	paused = true;
}

void PieceAnimation::init(unsigned long t) {
	Animation::init(t);
}

void PieceAnimation::update(unsigned long time) {
	Animation::update(time);
    
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

    glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
    float t = curTime / this->totalSpan;

	int currentSpan = getTimespanIndex(t);
	if(currentSpan == -1)
		return;
    
    if (t > 1.0 + T_INTERVAL)
        ended = true;
    
	glTranslatef(timeSpans.at(currentSpan).at(0), timeSpans.at(currentSpan).at(1), timeSpans.at(currentSpan).at(2));
    
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();

}

int PieceAnimation::getTimespanIndex(float time) {
	if ( time > 1.0 )
        time = 1.0;
        
    return time / T_INTERVAL;
}

float PieceAnimation::angleBetweenVectors(float vector1[3], float vector2[3]) {
	float ux = vector1[0];
	float uz = vector1[2];
	float vx = vector2[0];
	float vz = vector2[2];

	float angle = atan2(-uz * vx + ux * vz, ux * vx + uz * vz);
	angle *= (180 / M_PI);	

	return angle;
}
