#ifndef GLOBALATTRIBUTES_H
#define GLOBALATTRIBUTES_H

#include "InfoBlock.h"

class GlobalAttributes: public InfoBlock {
public:
	float red;
	float green;
	float blue;
	float alpha;

	string drawmode;
	string shading;
	string cullface;
	string cullorder;

	GlobalAttributes(vector<float> rgbaValues, vector<string> modes); //that string vector might be changed
	bool validate();
};

#endif