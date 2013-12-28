#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#define FREE_CAMERA 0
#define AUTO_CAMERA 1
#define CAMERA_MOVEMENT_TIME 15000
#define CAMERA_MOVEMENT_TIME_SECONDS 1.5

#include "CGFcamera.h"
#include "PositionPoint.h"

#define PLAYER1_ANGLE 0.0
#define PLAYER2_ANGLE 180.0
#define PLAYER1_SECOND_ANGLE 360.0

#define PLAYER1 0
#define PLAYER2 1

class CameraController {
	float focusingTo;
	CGFcamera* freeCamera;
	int enabledCamera;
	PositionPoint currentAutoCameraPos;
	float initialTime;
    float currentAngle;
    float startingAngle;
public:
    CGFcamera* autoCamera;
	bool isChangingFocus;
	bool focusChangeInitialized;
    int enabled;

	CameraController();
	CGFcamera* getEnabledCamera();
	int getEnabledCameraType();
	CGFcamera* getAutoCamera();
	CGFcamera* getFreeCamera();
	void changeCamera();
	void changePlayerFocus(int player);
	void initializeFocusChange(unsigned long t);
	void updateFocus(unsigned long t);
    void setAngle(float angle);
	void resetFreeCam();
};

#endif
