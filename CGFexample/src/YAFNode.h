#ifndef YAFNODE_H
#define YAFNODE_H

#include "YAFElement.h"
#include "ScenePrimitive.h"
#include "Transformation.h"

class YAFNode {
    
public:
    string id;
    vector<Transformation> transformations;
    string appearanceID;
    vector<ScenePrimitive> primitives;
};

#endif
