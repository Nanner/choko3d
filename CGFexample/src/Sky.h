#ifndef SKY_H
#define SKY_H

#define HALF_SIZE 0.5f

#include "ScenePrimitive.h"

class Sky : public ScenePrimitive {
public:
    Sky();
    void draw();
};

#endif