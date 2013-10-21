#ifndef _PLANE_H
#define _PLANE_H

#include "ScenePrimitive.h"

class Plane: public ScenePrimitive {
public:
    Plane(int parts);
    
    int parts;
    void init();
    void draw();
};

#endif
