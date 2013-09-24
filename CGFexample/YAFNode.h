#ifndef YAFNODE_H
#define YAFNODE_H

#include "YAFElement.h"
#include "Primitive.h"
#include "Transformation.h"

class YAFNode {
    
public:
    string id;
    vector<Transformation> transformations;
    string appearanceID;
    vector<Primitive> primitives;
};

#endif
