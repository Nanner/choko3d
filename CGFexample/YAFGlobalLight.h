#ifndef YAFGLOBALLIGHT_H
#define YAFGLOBALLIGHT_H

#include "YAFElement.h"

class YAFGlobalLight: public YAFElement {
public:
	bool doublesided;
	bool local;
	bool enabled;
	float ambientR;
	float ambientG;
	float ambientB;
	float ambientA;
    
	YAFGlobalLight(vector<bool> ligthingProperties, vector<float> ambientRGBA);
    void validate();
};

#endif
