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
	float camNear;
	float camFar;

	CameraView(string id, float near, float camFar);
    virtual void applyView() = 0;
	virtual void updateProjectionMatrix(int width, int height) = 0;
	virtual void resetCamera() = 0;
};

class Perspective: public CameraView {
public:
	float iPosition[3];
	float iAngle;
	float iCameraRotation[2];
	float iUpVector[3];
	float iCameraRadius;

	float angle;
	float cameraRotation[2]; //We assume the camera "orbits" around the target point, using spherical coordinates is easier for this
	float upVector[3];
	float cameraRadius;


	Perspective(string id, float near, float camFar, float angle, float posX, float posY, float posZ, float targetX, float targetY, float targetZ);

    void applyView();
	void updateProjectionMatrix(int width, int height);
	bool translate(int axis, float value);
	bool rotate(int axis, float angle);
	void resetCamera();
};

class Orthographic: public CameraView {
public:
	float iLeft;
	float iRight;
	float iTop;
	float iBottom;

	float left;
	float right;
	float top;
	float bottom;

	Orthographic(string id, float near, float camFar, float left, float right, float top, float bottom);

	void applyView();
	void updateProjectionMatrix(int width, int height);
	bool translate(int axis, float value);
	void resetCamera();
};

float distanceBetweenPoints(float point1[3], float point2[3]);

float radians(float degrees);
float degrees(float radians);

#endif