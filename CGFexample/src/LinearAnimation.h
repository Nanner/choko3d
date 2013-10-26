#ifndef LINEAR_ANIMATION_H
#define LINEAR_ANIMATION_H

#include "Animation.h"

class LinearAnimation : public Animation {
    
    
public:
    float span;
    vector<float> controlPoints;
    
    LinearAnimation(float span, vector<float> controlPoints);
    
    
};

#endif
