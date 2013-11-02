#ifndef LINEAR_ANIMATION_H
#define LINEAR_ANIMATION_H

#include <math.h>
#include "Animation.h"

class LinearAnimation : public Animation {
    
    
public:
	bool ended;
    float totalSpan;
	vector<float> timeSpans;

	float totalElapsedTime;
	float elapsedTimeInTraj;
	int currentTrajectory;

	int numTrajectories;
	float totalDist;
    vector<float> controlPoints;
	vector<float> trajectoryDists;
	vector<float> trajectoryCoordDeltas;
	vector<float> trajectoryCoordPreviousOffsets;
	vector<float> trajectoryAngles;
    
    LinearAnimation(float span, vector<float> controlPoints);
    
    void update(unsigned long t);

	void init(unsigned long t);

	int getTimespanIndex(unsigned long currentTime);

	void applyRotation();
};

//float distanceBetweenPoints(float point1[3], float point2[3]);

float angleBetweenVectors(float vector1[3], float len1, float vector2[3], float len2);

bool gluInvertMatrix(const float m[16], float invOut[16]);

#endif
