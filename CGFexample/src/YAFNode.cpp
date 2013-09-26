#include "YAFNode.h"

string YAFNode::rootID = "";

YAFNode::YAFNode(string id) {
    this->id = id;
    this->appearanceID = "";
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

YAFNode::~YAFNode(){
    
}