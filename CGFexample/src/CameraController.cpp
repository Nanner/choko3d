#include "CameraController.h"

CameraController::CameraController() {
	enabledCamera = AUTO_CAMERA;
	CGFcamera* freeCamera = new CGFcamera();
	//freeCamera->moveTo(1, 15, 0.01);
	this->freeCamera = freeCamera;
    
	CGFcamera* autoCamera = new CGFcamera();
	//autoCamera->setX(-10);
	//autoCamera->setY(-30);
	autoCamera->setZ(-90);
	autoCamera->rotate(CG_CGFcamera_AXIS_X, 45);
	this->autoCamera = autoCamera;
	focusChangeInitialized = false;
	isChangingFocus = false;
	foccusingTo = 180;
	autoCamera->setRotation(CG_CGFcamera_AXIS_Y, 0);
}

CGFcamera* CameraController::getEnabledCamera() {
	if(enabledCamera == AUTO_CAMERA)
		return autoCamera;
	else
		return freeCamera;
}

CGFcamera* CameraController::getAutoCamera() {
	return autoCamera;
}

CGFcamera* CameraController::getFreeCamera() {
	return freeCamera;
}

void CameraController::changeCamera() {
	if(enabledCamera == AUTO_CAMERA)
		enabledCamera = FREE_CAMERA;
	else
		enabledCamera = AUTO_CAMERA;
}

void CameraController::moveCameraToPoint(/*PositionPoint point*/) {
	autoCamera->moveTo(1, 50, 0.5);
}

void CameraController::changePlayerFocus() {
	isChangingFocus = true;
}

void CameraController::initializeFocusChange(unsigned long t) {
	initialTime = t;
	focusChangeInitialized = true;
}

void CameraController::updateFocus(unsigned long t) {
	float fraction = ((t - initialTime) / (float) CAMERA_MOVEMENT_TIME) * foccusingTo;
    
	if(fraction > foccusingTo)
		fraction = foccusingTo;
    
	if(autoCamera->rotateTo(CG_CGFcamera_AXIS_Y, foccusingTo, fraction) == true) {
		isChangingFocus = false;
		focusChangeInitialized = false;
		foccusingTo = 0;
	}
}