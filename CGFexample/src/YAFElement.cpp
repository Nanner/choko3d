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

bool YAFElement::validShininess(float shininess) {
	return (shininess >= SHININESS_MIN && shininess <= SHININESS_MAX);
}
