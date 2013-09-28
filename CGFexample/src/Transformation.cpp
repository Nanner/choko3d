#include "Transformation.h"

float * Transformation::calculateMatrix(vector<Transformation *> transformations) {
    float * matrix = new float[16];
    
    glPushMatrix();
    glLoadIdentity();
    
    
    for(int i = 0; i < transformations.size(); i++) {
        float * currentMatrix = transformations.at(i)->getMatrix();
        glMultMatrixf(currentMatrix);
        delete currentMatrix;
    }
    
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();
    
    return matrix;
}

Translation::Translation(vector<float> xyz) {
    this->x = xyz.at(0);
    this->y = xyz.at(1);
    this->z = xyz.at(2);
}

void Translation::validate() {}

float * Translation::getMatrix() {
    float * matrix = new float[16];
    
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(x, y, z);
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();
    
    return matrix;
}



Rotation::Rotation(string axis, float angle) {
    this->axis = axis.at(0);
    this->angle = angle;
    x = 0.0; y = 0.0; z = 0.0;
    
    if ( this->axis == X )
        x = 1.0;
    
    if ( this->axis == Y)
        y = 1.0;
    
    if ( this->axis == Z)
        z = 1.0;
    
    this->validate();
}

void Rotation::validate() {
    if ( axis != X && axis != Y && axis != Z) {
        throw InvalidAttributeValueException("Rotation axis");
    }
    
    if ( !validAngle(angle) ) {
        throw InvalidAttributeValueException("Rotation angle");
    }
}

float * Rotation::getMatrix() {
    float * matrix = new float[16];
    
    glPushMatrix();
    glLoadIdentity();
    glRotatef(angle, x, y, z);
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();
    
    return matrix;
}



Scaling::Scaling(vector<float> factor){
    this->x = factor.at(0);
    this->y = factor.at(1);
    this->z = factor.at(2);
}

void Scaling::validate() {}

float * Scaling::getMatrix() {
    float * matrix = new float[16];
    
    glPushMatrix();
    glLoadIdentity();
    glScalef(x, y, z);
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();
    
    return matrix;
}
