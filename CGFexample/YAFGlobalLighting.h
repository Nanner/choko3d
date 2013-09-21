#ifndef YAFGLOBALLIGHTING_H
#define YAFGLOBALLIGHT_H

#include "YAFElement.h"

class YAFGlobalLighting: public YAFElement {
public:
	bool doublesided;
	bool local;
	bool enabled;
	float ambientR;
	float ambientG;
	float ambientB;
	float ambientA;
    
    YAFGlobalLighting();
	YAFGlobalLighting(vector<bool> ligthingAttributes, vector<float> ambientRGBA);
    void validate();
};

#endif
