#ifndef _CAMERAVIEW_H
#define _CAMERAVIEW_H

#include "CGFcamera.h"
#include "CGFscene.h"
#include <string>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

class CameraView: public CGFcamera {
public:
	float aspect;
	string id;
	float near;
	float far;

	CameraView(string id, float near, float far);
    void virtual applyView() = 0;
};

class Perspective: public CameraView {
public:
	float angle;
	float cameraRotation[2]; //We assume the camera "orbits" around the target point, using spherical coordinates is easier for this
	float upVector[3];
	float cameraRadius;

	Perspective(string id, float near, float far, float angle, float posX, float posY, float posZ, float targetX, float targetY, float targetZ);

    void applyView();
	bool translate(int axis, float value);
	bool rotate(int axis, float angle);
};

class Orthographic: public CameraView {
public:
	float left;
	float right;
	float top;
	float bottom;

	Orthographic(string id, float near, float far, float left, float right, float top, float bottom);

	void applyView();
	bool translate(int axis, float value);
};

float distanceBetweenPoints(float point1[3], float point2[3]);

float radians(float degrees);
float degrees(float radians);

#endif