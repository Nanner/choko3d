#ifndef _CAMERAVIEW_H
#define _CAMERAVIEW_H

#include "CGFcamera.h"
#include "CGFscene.h"
#include <string>

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

	Perspective(string id, float near, float far, float angle, float posX, float posY, float posZ, float targetX, float targetY, float targetZ);

    void applyView();
};

class Orthographic: public CameraView {
public:
	float left;
	float right;
	float top;
	float bottom;

	Orthographic(string id, float near, float far, float left, float right, float top, float bottom);

	void applyView();
};

#endif