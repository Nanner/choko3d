#ifndef PIECE_ANIMATION_H
#define PIECE_ANIMATION_H

#include <math.h>
#include "Animation.h"

class PieceAnimation : public Animation {
    

public:
	static bool pieceIsMoving;
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
    
    PieceAnimation(float span, vector<float> controlPoints);
    
    void update(unsigned long t);

	void init(unsigned long t);

	int getTimespanIndex(unsigned long currentTime);

	void applyRotation();
    
    float angleBetweenVectors(float vector1[3], float vector2[3]);
};

#endif
