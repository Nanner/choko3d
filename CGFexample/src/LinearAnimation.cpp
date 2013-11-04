#include "LinearAnimation.h"

LinearAnimation::LinearAnimation(float span, vector<float> controlPoints): Animation(){
    this->totalSpan = span * 1000;
    this->controlPoints = controlPoints;
	this->numTrajectories = (this->controlPoints.size() / 3) - 1;
	this->totalDist = 0;

	for(unsigned int i = 0; i < numTrajectories; i++) {
		float point1[3], point2[3];
		int ind1 = (i * 3);
		int ind2 = ((i+1) * 3);
		point1[0] = controlPoints.at(0 + ind1);
		point1[1] = controlPoints.at(1 + ind1);
		point1[2] = controlPoints.at(2 + ind1);

		point2[0] = controlPoints.at(0 + ind2);
		point2[1] = controlPoints.at(1 + ind2);
		point2[2] = controlPoints.at(2 + ind2);

		float trajDist = distanceBetweenPoints(point1, point2);
		totalDist += trajDist;
		trajectoryDists.push_back(trajDist);
	}

	for(unsigned int i = 0; i < numTrajectories; i++) {
		float deltaX;
		float deltaY;
		float deltaZ;

		int ind1 = (i * 3);
		int ind2 = ((i+1) * 3);
		float point1X = controlPoints.at(0 + ind1);
		float point1Y = controlPoints.at(1 + ind1);
		float point1Z = controlPoints.at(2 + ind1);

		float point2X = controlPoints.at(0 + ind2);
		float point2Y = controlPoints.at(1 + ind2);
		float point2Z = controlPoints.at(2 + ind2);

		deltaX = point2X - point1X;
		deltaY = point2Y - point1Y;
		deltaZ = point2Z - point1Z;

		trajectoryCoordDeltas.push_back(deltaX);
		trajectoryCoordDeltas.push_back(deltaY);
		trajectoryCoordDeltas.push_back(deltaZ);

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

		if(rotAngle > 360)
			rotAngle -= 360;
		else if(rotAngle < 0)
			rotAngle += 360;

		rotAngle += previousAngle;

		trajectoryAngles.push_back(rotAngle);
		

	}

	currentTrajectory = 0;
	elapsedTimeInTraj = 0;
	ended = false;
}

void LinearAnimation::init(unsigned long t) {
	for(unsigned int i = 0; i < numTrajectories; i++) {
		float distFraction = trajectoryDists.at(i) / totalDist;
		unsigned long timeSpan = (distFraction * totalSpan);
		timeSpans.push_back(timeSpan);
		//printf("%i - %lu\n", i, timeSpan );
	}
	//printf("current time: %lu\n", t);
	Animation::init(t);
	totalElapsedTime = 0;
}

void LinearAnimation::update(unsigned long t) {

	if(ended)
		return;

	float curTime = t - this->startTime;

	//printf("Time: %f\n", curTime);

    glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	//printf("current time: %lu\n", curTime);
    
    //float distance = curTime * 0.0001;
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
	if(currentTimeFragment == 1 && currentTrajectory == (numTrajectories - 1))
		ended = true;

	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();
}

int LinearAnimation::getTimespanIndex(unsigned long currentTime) {
	float timePart = 0;
	for(unsigned int i = 0; i < timeSpans.size(); i++) {
		if(currentTime <= timePart + timeSpans.at(i))
			return i;

		timePart += timeSpans.at(i);
	}

	return timeSpans.size() - 1;
}

void LinearAnimation::applyRotation() {

	if(currentTrajectory != 0) {
		glRotatef(trajectoryAngles.at(currentTrajectory - 1), 0, 1.0, 0);
	}
}

/*float distanceBetweenPoints(float point1[3], float point2[3]) {
	return sqrt((point2[0]-point1[0])*(point2[0]-point1[0]) + (point2[1]-point1[1])*(point2[1]-point1[1]) + (point2[2]-point1[2])*(point2[2]-point1[2]));
}*/

float angleBetweenVectors(float vector1[3], float vector2[3]) {

	float ux = vector1[0];
	float uz = vector1[2];
	float vx = vector2[0];
	float vz = vector2[2];

	float angle = atan2(-uz * vx + ux * vz, ux * vx + uz * vz);
	angle *= (180 / M_PI);	

	return angle;
}
