#ifndef CURVED_ANIMATION_H
#define CURVED_ANIMATION_H

#include "Animation.h"

class CurvedAnimation : public Animation {
public:
	CurvedAnimation(float span, vector<float> controlPoints);

	float totalSpan;
	vector<float> controlPoints;
	vector<float> P0;
	vector<float> P1;
	vector<float> P2;
	vector<float> P3;

	void update(unsigned long t);
};

#endif