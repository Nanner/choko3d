#ifndef _PATCH_H
#define _PATCH_H

#include "ScenePrimitive.h"

class Point;

class Patch: public ScenePrimitive {
public:
    Patch(int order, int partsU, int partsV, string compute, vector<float> controlPoints);
    
    int order;
    int partsU;
    int partsV;
    int compute;
    vector<float> controlPoints;
    static GLfloat texels[4][2];
    
    void draw();
};

#endif
