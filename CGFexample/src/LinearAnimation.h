#ifndef LINEAR_ANIMATION_H
#define LINEAR_ANIMATION_H

#include "Animation.h"

class LinearAnimation : public Animation {
    
    
public:
    float totalSpan;
	vector<float> timeSpans;

	int numTrajectories;
	float totalDist;
    vector<float> controlPoints;
	vector<float> trajectoryDists;
	vector<float> trajectoryCoordDeltas;
    
    LinearAnimation(float span, vector<float> controlPoints);
    
    void update(unsigned long t);

	void init(unsigned long t);

	int getTimespanIndex(unsigned long currentTime);
};

//float distanceBetweenPoints(float point1[3], float point2[3]);

#endif
