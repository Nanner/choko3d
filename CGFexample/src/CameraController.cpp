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
	focusingTo = 0.0;
    currentAngle = 0.0;
    startingAngle = 0.0;
	autoCamera->setRotation(CG_CGFcamera_AXIS_Y, 0);
    enabled = true;
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

void CameraController::changePlayerFocus(int player) {
    if (enabled) {
        isChangingFocus = true;
        
        if (player == PLAYER1) {
            focusingTo = PLAYER1_SECOND_ANGLE;
        } else {
            focusingTo = PLAYER2_ANGLE;
        }
    }
}

void CameraController::setAngle(float angle) {
    autoCamera->setRotation(CG_CGFcamera_AXIS_Y, angle);
    currentAngle = angle;
    startingAngle = angle;
}

void CameraController::initializeFocusChange(unsigned long t) {
	initialTime = t;
	focusChangeInitialized = true;
    startingAngle = currentAngle;
}

void CameraController::updateFocus(unsigned long t) {
	currentAngle += ((t - initialTime) / (float) CAMERA_MOVEMENT_TIME) * (focusingTo - startingAngle);
    
    if (currentAngle >= focusingTo)
        currentAngle = focusingTo;
    
	autoCamera->setRotation(CG_CGFcamera_AXIS_Y, currentAngle);
    
    if (currentAngle >= focusingTo) {
        if (focusingTo == PLAYER1_SECOND_ANGLE) {
            autoCamera->setRotation(CG_CGFcamera_AXIS_Y, PLAYER1_ANGLE);
            focusingTo = 0.0;
            currentAngle = 0.0;
        }
		isChangingFocus = false;
		focusChangeInitialized = false;
	}
}