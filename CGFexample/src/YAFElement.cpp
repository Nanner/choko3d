#include "YAFElement.h"

bool YAFElement::validRGBA(float red, float green, float blue, float alpha){
    if((red < RGBA_LOWER_BOUND) || (red > RGBA_UPPER_BOUND)
       || (green < RGBA_LOWER_BOUND) || (green > RGBA_UPPER_BOUND)
       || (blue < RGBA_LOWER_BOUND) || (blue > RGBA_UPPER_BOUND)
       || (alpha < RGBA_LOWER_BOUND) || (alpha > RGBA_UPPER_BOUND))
		return false;
    else
        return true;
}

bool YAFElement::validXYZ(float x, float y, float z) {
    if(x < XYZ_MIN || x > XYZ_MAX)
        return false;
    if(y < XYZ_MIN || y > XYZ_MAX)
        return false;
    if(z < XYZ_MIN || z > XYZ_MAX)
        return false;
    
    return true;
}

bool YAFElement::validAngle(float angle) {
    return (angle >= ANGLE_MIN && angle <= ANGLE_MAX);
}

bool YAFElement::validMinMax(float min, float max) {
    return (min >= XYZ_MIN && max <= XYZ_MAX);
}