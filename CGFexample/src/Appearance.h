#ifndef _APPEARANCE_H
#define _APPEARANCE_H

#include "CGFappearance.h"
#include "CGFapplication.h"

class Appearance: public CGFappearance {
protected:
	static unsigned int currentID;
	float texLength_s;
	float texLength_t;
	float emissivity[4];

public:
	static int currentTextIndex;

	unsigned int id;
	float getTexLength_s();
	float getTexLength_t();
	void setTexture(string t);
	void setTexLength_s(float s);
	void setTexLength_t(float t);
	void apply();
	Appearance(float* a, float* d, float* sp, float* emis, float sh);
};

#endif