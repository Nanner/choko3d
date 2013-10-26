#ifndef _PLANE_H
#define _PLANE_H

#include "ScenePrimitive.h"

class Plane: public ScenePrimitive {
public:
    Plane(int parts);
    static GLfloat ctrlpoints[4][3];
    static GLfloat nrmlcompon[4][3];
    static GLfloat colorpoints[4][4];
    static GLfloat textpoints[4][2];
    
    int parts;
    void draw();
};

#endif
