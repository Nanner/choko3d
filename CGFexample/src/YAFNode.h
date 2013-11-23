#ifndef YAFNODE_H
#define YAFNODE_H

#include "YAFElement.h"
#include "ScenePrimitive.h"
#include "Transformation.h"

class YAFNode {
    
public:
    string id;
    float * transformationMatrix;
    string appearanceID;
    vector<ScenePrimitive *> primitives;
    vector<string> nodeReferences;
    
    bool usesDisplayList;
    
    string animationID;
    
    static string rootID;
    
	YAFNode() {};
    YAFNode(string id);
    void setTransformations(vector<Transformation *> transformations);
    void setAppearanceID(string id);
    void addPrimitive(ScenePrimitive * primitive);
    void addNodeReference(string nodeID);
    
    void setDisplayList(bool usesDisplayList);
    
    void setAnimationID(string id);
    
    ~YAFNode();
};

#endif
