#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#define FREE_CAMERA 0
#define AUTO_CAMERA 1
#define CAMERA_MOVEMENT_TIME 5000

#include "CGFcamera.h"
#include "Game.h"

class CameraController {
	float foccusingTo;
	CGFcamera* freeCamera;
	CGFcamera* autoCamera;
	int enabledCamera;
	PositionPoint currentAutoCameraPos;
	float initialTime;
public:
	bool isChangingFocus;
	bool focusChangeInitialized;
	CameraController();
	CGFcamera* getEnabledCamera();
	CGFcamera* getAutoCamera();
	CGFcamera* getFreeCamera();
	void changeCamera();
	void moveCameraToPoint(/*PositionPoint point*/);
	void changePlayerFocus();
	void initializeFocusChange(unsigned long t);
	void updateFocus(unsigned long t);
};

#endif
