#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "YAFElement.h"
#include <GL/glui.h>

#define X 'x'
#define Y 'y'
#define Z 'z'

using namespace std;

class Transformation : public YAFElement {
public:
    virtual ~Transformation(){};
    static float * calculateMatrix(vector<Transformation *> transformations);
    
private:
    virtual float * getMatrix() = 0;
};



class Translation : public Transformation {
    
public:
    float x;
    float y;
    float z;
    Translation(vector<float> xyz);
    
    void validate();
    float * getMatrix();
    virtual ~Translation(){};
};



class Rotation : public Transformation {
    
public:
    char axis;
    float angle;
    float x, y, z;
    Rotation(string axis, float angle);
    
    void validate();
    float * getMatrix();
    virtual ~Rotation(){};
};



class Scaling : public Transformation {
public:
    float x;
    float y;
    float z;
    Scaling(vector<float> factor);
    
    void validate();
    float * getMatrix();
    virtual ~Scaling(){};
};



#endif 
