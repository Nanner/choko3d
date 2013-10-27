#ifndef ANIMATION_H
#define ANIMATION_H

#include "ScenePrimitive.h"

class Animation {
protected:
    float * matrix;
public:
    float * getMatrix();
    unsigned long startTime;
    unsigned char doReset;
    void init(unsigned long t);
    virtual void update(unsigned long t) {};
    void reset();
    Animation();
};

#endif