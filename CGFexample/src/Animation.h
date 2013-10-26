#ifndef ANIMATION_H
#define ANIMATION_H

#include "ScenePrimitive.h"

class Animation {
public:
    vector<ScenePrimitive *> objects;
    unsigned long startTime;
    unsigned char doReset;
    void init(unsigned long t);
    void update(unsigned long t);
    void draw();
    void reset();
    Animation();
};

#endif