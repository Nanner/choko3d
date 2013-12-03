#ifndef ANIMATION_H
#define ANIMATION_H

//#include "ScenePrimitive.h"
#include "CGFscene.h"
#include "CameraView.h" //For M_PI and distanceBetweenPoints

class Animation {
protected:
    float * matrix;
public:
	bool isInitialized;
	bool loop;
	bool paused;
	unsigned long startTime;
	unsigned long pauseStartTime;
	unsigned long pausedTime;

    float * getMatrix();
    virtual void init(unsigned long t);
    virtual void update(unsigned long t) {}
	virtual void applyRotation() {}
    virtual void reset();
	virtual void pause();
	virtual void resume();
    Animation();
};

#endif