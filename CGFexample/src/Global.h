#ifndef GLOBALATTRIBUTES_H
#define GLOBALATTRIBUTES_H

#include "YAFElement.h"

class Global: public YAFElement {
public:
	float red;
	float green;
	float blue;
	float alpha;

	string drawmode;
	string shading;
	string cullface;
	string cullorder;

	Global(){}
	Global(vector<float> rgbaValues, vector<string> backgroundAttributes);
	void validate();
};

#endif