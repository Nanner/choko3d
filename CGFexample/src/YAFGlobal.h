#ifndef YAFGLOBAL_H
#define YAFGLOBAL_H

#include "YAFElement.h"

class YAFGlobal: public YAFElement {
public:
	float red;
	float green;
	float blue;
	float alpha;

	string drawmode;
	string shading;
	string cullface;
	string cullorder;

	YAFGlobal(){}
	YAFGlobal(vector<float> rgbaValues, vector<string> backgroundAttributes);
	void validate();
};

#endif