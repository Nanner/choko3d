#include "YAFElement.h"

bool YAFElement::validRGBA(float red, float green, float blue, float alpha){
    if((red < RGBA_MIN) || (red > RGBA_MAX)
       || (green < RGBA_MIN) || (green > RGBA_MAX)
       || (blue < RGBA_MIN) || (blue > RGBA_MAX)
       || (alpha < RGBA_MIN) || (alpha > RGBA_MAX))
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

bool YAFElement::validShininess(float shininess) {
	return (shininess >= SHININESS_MIN && shininess <= SHININESS_MAX);
}
