#ifndef PIECE_ANIMATION_H
#define PIECE_ANIMATION_H

#include <math.h>
#include <queue>
#include "Animation.h"

#define T_INTERVAL 0.02

class PieceAnimation : public Animation {
	static queue<PieceAnimation*> globalPieceAnimations;
    
public:
	static void updatePieceAnimations(unsigned long t);
	static void addPieceAnimation(PieceAnimation* pieceAnimation);
	static bool pendingAnimations();
	static void clearGlobalPieceAnimations();

    float totalSpan;
	vector<vector<float> > timeSpans;
    
    PieceAnimation(float span, vector<float> controlPoints);
    
    void update(unsigned long t);

	void init(unsigned long t);

	int getTimespanIndex(float time);
    
    float angleBetweenVectors(float vector1[3], float vector2[3]);
    
    vector<float> P0, P1, P2, P3;
};

#endif
