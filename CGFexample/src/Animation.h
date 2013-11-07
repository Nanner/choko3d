#ifndef ANIMATION_H
#define ANIMATION_H

#include "ScenePrimitive.h"

class Animation {
protected:
    float * matrix;
public:
	bool isInitialized;
    float * getMatrix();
    unsigned long startTime;
    unsigned char doReset;
    virtual void init(unsigned long t);
    virtual void update(unsigned long t) {}
	virtual void applyRotation() {}
    void reset();
    Animation();
};

#endif