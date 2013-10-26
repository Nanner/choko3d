#include "YAFNode.h"

string YAFNode::rootID = "";

YAFNode::YAFNode(string id) {
    this->id = id;
    this->appearanceID = "";
    this->animationID = "";
    this->usesDisplayList = false;
}

void YAFNode::setTransformations(vector<Transformation *> transformations) {
    transformationMatrix = Transformation::calculateMatrix(transformations);
}

void YAFNode::setAppearanceID(string id) {
    appearanceID = id;
}

void YAFNode::addPrimitive(ScenePrimitive * primitive) {
    primitives.push_back(primitive);
}

void YAFNode::addNodeReference(string nodeID) {
    nodeReferences.push_back(nodeID);
}

void YAFNode::setDisplayList(bool usesDisplayList) {
    this->usesDisplayList = usesDisplayList;
}

void YAFNode::setAnimationID(string id) {
    this->animationID = id;
}

YAFNode::~YAFNode(){
    
}