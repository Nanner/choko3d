#include "PieceAnimation.h"

queue<PieceAnimation*> PieceAnimation::globalPieceAnimations;

void PieceAnimation::updatePieceAnimations(unsigned long t) {
	if(globalPieceAnimations.empty())
		return;
	PieceAnimation* currentAnimation = globalPieceAnimations.front();
	if(currentAnimation->ended)
		globalPieceAnimations.pop();

	if(globalPieceAnimations.empty())
		return;

	currentAnimation = globalPieceAnimations.front();
	currentAnimation->resume();
	if (currentAnimation->isInitialized)
		currentAnimation->update(t);
	else
		currentAnimation->init(t);
}

void PieceAnimation::addPieceAnimation(PieceAnimation* pieceAnimation) {
	globalPieceAnimations.push(pieceAnimation);
}

bool PieceAnimation::pendingAnimations() {
	if(globalPieceAnimations.empty() || (globalPieceAnimations.size() == 1 && globalPieceAnimations.front()->ended)) {
		return false;
	}

	return true;
}

PieceAnimation::PieceAnimation(float span, vector<float> controlPoints): Animation(){
    this->totalSpan = span * 1000;
    this->controlPoints = controlPoints;
	this->numTrajectories = (this->controlPoints.size() / 3) - 1;
	this->totalDist = 0;

	//Load trajectory distances, calculate total distance and coordinate deltas between trajectories
	for(unsigned int i = 0; i < numTrajectories; i++) {
		//Get this point and the next
		float point1[3], point2[3];
		int ind1 = (i * 3);
		int ind2 = ((i+1) * 3);
		point1[0] = controlPoints.at(0 + ind1);
		point1[1] = controlPoints.at(1 + ind1);
		point1[2] = controlPoints.at(2 + ind1);

		point2[0] = controlPoints.at(0 + ind2);
		point2[1] = controlPoints.at(1 + ind2);
		point2[2] = controlPoints.at(2 + ind2);

		//Calculate distance between those 2 points and add it to total distance and trajectory distances vector
		float trajDist = distanceBetweenPoints(point1, point2);
		totalDist += trajDist;
		trajectoryDists.push_back(trajDist);

		//Calculate deltas between coordinates of the two points and add them to deltas vector
		float deltaX;
		float deltaY;
		float deltaZ;

		deltaX = point2[0] - point1[0];
		deltaY = point2[1] - point1[1];
		deltaZ = point2[2] - point1[2];

		trajectoryCoordDeltas.push_back(deltaX);
		trajectoryCoordDeltas.push_back(deltaY);
		trajectoryCoordDeltas.push_back(deltaZ);


		//Calculate the previous offsets for the trajectory coordinates (that is, the sum of the previous deltas), and push them to the vector
		int previousIndex;

		if(i == 0) {
			trajectoryCoordPreviousOffsets.push_back(0);
			trajectoryCoordPreviousOffsets.push_back(0);
			trajectoryCoordPreviousOffsets.push_back(0);

			previousIndex = 0;
		}
		else
			previousIndex = i*3;

		if(i < (numTrajectories - 1) ) {

			trajectoryCoordPreviousOffsets.push_back(deltaX + trajectoryCoordPreviousOffsets.at(previousIndex + 0));
			trajectoryCoordPreviousOffsets.push_back(deltaY + trajectoryCoordPreviousOffsets.at(previousIndex + 1));
			trajectoryCoordPreviousOffsets.push_back(deltaZ + trajectoryCoordPreviousOffsets.at(previousIndex + 2));
		}
	}

	//Calculate angles between the trajectories based on the trajectory vector components on the XZ plane, always summing the angle with the previous one
	for(unsigned int i = 0; i < numTrajectories - 1; i++) {
		float previousAngle;
		if(i == 0)
			previousAngle = 0;
		else
			previousAngle = trajectoryAngles.at(i - 1);

		int ind = i * 3;
		int ind2 = i * 3 + 3;
		float vector1[3];
		vector1[0] = trajectoryCoordDeltas.at(ind + 0);

		//Since we want the vector projections in the XZ plane, we consider y = 0
		vector1[1] = 0;
		vector1[2] = trajectoryCoordDeltas.at(ind + 2);

		float vector2[3];
		vector2[0] = trajectoryCoordDeltas.at(ind2 + 0);

		//Since we want the vector projections in the XZ plane, we consider y = 0
		vector2[1] = 0;
		vector2[2] = trajectoryCoordDeltas.at(ind2 + 2);

		float rotAngle = angleBetweenVectors(vector2, vector1);
		
		//To ensure we use angles between 0 and 360, for simplicity
		if(rotAngle > 360)
			rotAngle -= 360;
		else if(rotAngle < 0)
			rotAngle += 360;

		rotAngle += previousAngle;
		
		trajectoryAngles.push_back(rotAngle);
	}

	// Using the trajectory distances and the total distance, calculate the fraction of total animation
	//that each trajectory represents, and thus calculate the different time spans each trajectory
	//shall need to complete its animation.
	for(unsigned int i = 0; i < numTrajectories; i++) {
		float distFraction = trajectoryDists.at(i) / totalDist;
		unsigned long timeSpan = (distFraction * totalSpan);
		timeSpans.push_back(timeSpan);
	}

	currentTrajectory = 0;
	elapsedTimeInTraj = 0;
	totalElapsedTime = 0;
	paused = true;
}

void PieceAnimation::init(unsigned long t) {
	Animation::init(t);
	currentTrajectory = 0;
	elapsedTimeInTraj = 0;
	totalElapsedTime = 0;
}

void PieceAnimation::update(unsigned long t) {
	Animation::update(t);
    
	if(ended && loop) {
		reset();
		return;
	}
	else if(ended) {
		this->paused = true;
		this->pausedTime = 0;
		this->pauseStartTime = 0;
		return;
	}

	float curTime = t - this->startTime - pausedTime;

    glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	int currentTraj = getTimespanIndex(curTime);
	if(currentTraj == -1)
		return;

	//If we moved on to a new trajectory
	if(currentTraj != currentTrajectory) {
		//Reset elapsed time
		elapsedTimeInTraj = 0;

		//Update the indicator
		currentTrajectory = currentTraj;
	}
	else
		elapsedTimeInTraj += curTime - totalElapsedTime;

	totalElapsedTime = curTime;

	unsigned int ind = (currentTrajectory * 3);

	float currentTimeFragment = elapsedTimeInTraj / timeSpans.at(currentTrajectory);

	//This allows us to finish the animation in case the application is interrupted for longer after the animation would be over
	if(currentTimeFragment > 1)
		currentTimeFragment = 1;

	glTranslatef(trajectoryCoordPreviousOffsets.at(ind + 0) + trajectoryCoordDeltas.at(ind + 0) * currentTimeFragment,
		trajectoryCoordPreviousOffsets.at(ind + 1) + trajectoryCoordDeltas.at(ind + 1) * currentTimeFragment,
		trajectoryCoordPreviousOffsets.at(ind + 2) + trajectoryCoordDeltas.at(ind + 2) * currentTimeFragment);


	//If we are already on the last fragment of the last trajectory, indicate that the animation is over
	if(currentTimeFragment == 1 && currentTrajectory == (numTrajectories - 1)) {
		ended = true;
	}

	//glTranslatef(controlPoints.at(0), controlPoints.at(1), controlPoints.at(2));

	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();
}

int PieceAnimation::getTimespanIndex(unsigned long currentTime) {
	float timePart = 0;
	for(unsigned int i = 0; i < timeSpans.size(); i++) {
		if(currentTime <= timePart + timeSpans.at(i))
			return i;

		timePart += timeSpans.at(i);
	}

	return timeSpans.size() - 1;
}

void PieceAnimation::applyRotation() {
	if(currentTrajectory != 0) {
		glRotatef(trajectoryAngles.at(currentTrajectory - 1), 0, 1.0, 0);
	}
}

float PieceAnimation::angleBetweenVectors(float vector1[3], float vector2[3]) {
	float ux = vector1[0];
	float uz = vector1[2];
	float vx = vector2[0];
	float vz = vector2[2];

	float angle = atan2(-uz * vx + ux * vz, ux * vx + uz * vz);
	angle *= (180 / M_PI);	

	return angle;
}
