#include "CameraView.h"
#include <iostream>

CameraView::CameraView(string id, float camNear, float camFar): id(id), camNear(camNear), camFar(camFar) {
	
	//Calculate the aspect ratio of the scene/app viewport
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	aspect = (float) m_viewport[2] / (float) m_viewport[3];
}

Perspective::Perspective(string id, float near, float camFar, float angle, float posX, float posY, float posZ, float targetX, float targetY, float targetZ):
	CameraView(id, near, camFar), angle(angle), iAngle(angle) {
		position[0] = iPosition[0] = posX;
		position[1] = iPosition[1] = posY;
		position[2] = iPosition[2] = posZ;

		target[0] = targetX;
		target[1] = targetY;
		target[2] = targetZ;

		//Calculate distance between the position of the camera and the target
		//This will be our camera radius, that is, the radius of a sphere with center on the target
		cameraRadius = iCameraRadius = distanceBetweenPoints(position, target);

		//Now we need to initiate the spherical coordinates according to our initial position
		float radianPhi = acos(-(position[1] - target[1]) / cameraRadius);
		cameraRotation[0] = 90 - degrees(radianPhi);
		float radianTheta = 0;
		if(radianPhi != 0 && radianPhi != M_PI) {
			radianTheta = asin((position[0] - target[0]) / (cameraRadius*sin(radianPhi)));
		}
		cameraRotation[1] = degrees(radianTheta);

		//We start the up vector in the direction of yy axis, unless the camera has the same x and z coordinates as the target
		if(position[0] == target[0] && position[2] == target[2] && position[1] > target[1]) {
			upVector[0] = 0.0;
			upVector[1] = 0.0;
			upVector[2] = -1.0;
		}
		else if(position[0] == target[0] && position[2] == target[2] && position[1] < target[1]) {
			upVector[0] = 0.0;
			upVector[1] = 0.0;
			upVector[2] = 1.0;
		}
		else {
			upVector[0] = 0.0;
			upVector[1] = 1.0;
			upVector[2] = 0.0;
		}

		iCameraRotation[0] = cameraRotation[0];
		iCameraRotation[1] = cameraRotation[1];
		iUpVector[0] = upVector[0];
		iUpVector[1] = upVector[1];
		iUpVector[2] = upVector[2];

}

void Perspective::applyView() {

	//Load the camera view projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle, aspect, camNear, camFar);

	//Load the camera position related matrix transformations
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(position[0], position[1], position[2], target[0], target[1], target[2], upVector[0], upVector[1], upVector[2]);
}

void Perspective::updateProjectionMatrix(int width, int height) {
	aspect = (float) width / (float) height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle, aspect, camNear, camFar);
}

bool Perspective::rotate(int axis, float angle) {
	if (axis!=CG_CGFcamera_AXIS_X && axis!=CG_CGFcamera_AXIS_Y) return false;
    
	float newCamRot[2];
	newCamRot[0] = cameraRotation[0];
	newCamRot[1] = cameraRotation[1];
	newCamRot[axis] += angle; //AXIS X rotation determines zenith angle (phi), while AXIS Y rotation determines azimuth angle (theta)

	//To ensure that we don't "overflow" our cameraRotation array, we bind the values from -360 to 360
	if(newCamRot[axis] < -360) {
		newCamRot[axis] += 360;
	}
	else if(newCamRot[axis] > 360) {
		newCamRot[axis] -= 360;
	}

	//Now we convert our angles to radians, and we take into account that the zenith is the angle between YY and Target-Position vectors,
	//not the angle between Target-Position and the XZ plane. So we need to subtract our cameraRotation[0] from 90º
	float radianPhi = radians(90-newCamRot[0]);
	float radianTheta = radians(newCamRot[1]);

	//Now, using spherical to cartesian coordinates conversion, and taking into account that the openGL axis work in a slightly different
	//way from the axis we're used to, we calculate the new position for the camera
	float pos0 = target[0] + cameraRadius*sin(radianPhi)*sin(radianTheta);
	float pos1 = target[1] - cameraRadius*cos(radianPhi);
	float pos2 = target[2] + cameraRadius*sin(radianPhi)*cos(radianTheta);

	//We choose to ignore whenever the user positions the camera directly on top or below the target,
	//that is, when the x and z coordinates of the two points match. By doing so, we avoid nasty problems
	//with the upvector suddenly changing and so we avoid some complicated calculations.
	if(pos0 != target[0] || pos2 != target[2]) {
		position[0] = pos0;
		position[1] = pos1;
		position[2] = pos2;

		upVector[0] = 0;
		upVector[1] = 0;
		upVector[2] = 0;

		cameraRotation[axis] = newCamRot[axis];

		//When the camera angle zenith is between -270 and -90 or between 90 and 270 (this, in practice, is the same
		//interval, but due to our way of manipulating angles, it's useful do define both of them)
		//the up vector must be inverted because our camera will be upside down.
		if(cameraRotation[CG_CGFcamera_AXIS_X] <= -90 && cameraRotation[CG_CGFcamera_AXIS_X] >= -270)
			upVector[1] = -1;
		else if(cameraRotation[CG_CGFcamera_AXIS_X] >= 90 && cameraRotation[CG_CGFcamera_AXIS_X] <= 270)
			upVector[1] = -1;
		else
			upVector[1] = 1;
	}

	return true;
}

bool Perspective::translate(int axis, float value) {
	if (axis!=CG_CGFcamera_AXIS_X && axis!=CG_CGFcamera_AXIS_Y && axis!=CG_CGFcamera_AXIS_Z) return false;

	//If the movement is in the Z axis, means we want to zoom in on the scene
	if(axis == CG_CGFcamera_AXIS_Z) {
		float newAngle = angle + 2 * value;
		if(newAngle > 0 && newAngle < 180)
			angle = newAngle;
	}

	return true;
}

void Perspective::resetCamera(){
	position[0] = iPosition[0];
	position[1] = iPosition[1];
	position[2] = iPosition[2];

	angle = iAngle;
	cameraRadius = iCameraRadius;

	cameraRotation[0] = iCameraRotation[0];
	cameraRotation[1] = iCameraRotation[1];

	upVector[0] = iUpVector[0];
	upVector[1] = iUpVector[1];
	upVector[2] = iUpVector[2];
}

Orthographic::Orthographic(string id, float near, float camFar, float left, float right, float top, float bottom):
	CameraView(id, near, camFar),iLeft(left), iRight(right), iTop(top), iBottom(bottom), left(left), right(right), top(top), bottom(bottom) {}

void Orthographic::applyView() {

	//Load the camera view projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left*aspect, right*aspect, bottom, top, camNear, camFar);

	//Same as above, model matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0,-1.0, 0.0, 1.0, 0.0);
	glRotatef(rotation[0], 1.f, 0.f, 0.f);
	glRotatef(rotation[1], 0.f, 1.f, 0.f);
	glRotatef(rotation[2], 0.f, 0.f, 1.f);
}

void Orthographic::updateProjectionMatrix(int width, int height) {
	aspect = (float) width / (float) height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left*aspect, right*aspect, bottom, top, camNear, camFar);
}

bool Orthographic::translate(int axis, float value) {
	if (axis!=CG_CGFcamera_AXIS_X && axis!=CG_CGFcamera_AXIS_Y && axis!=CG_CGFcamera_AXIS_Z) return false;

	if(axis == CG_CGFcamera_AXIS_X) {
		left -= value;
		right -= value;
	}

	if(axis == CG_CGFcamera_AXIS_Y) {
		top -= value;
		bottom -= value;
	}

	//If the movement is in the Z axis, means we want to zoom in on the scene
	if(axis == CG_CGFcamera_AXIS_Z) {

		float newLeft = left - value*2;
		float newRight = right + value*2;
		float newTop = top + value*2;
		float newBottom = bottom - value*2;

		if(newLeft < newRight && newBottom < newTop) {
			left = newLeft;
			right = newRight;
			top = newTop;
			bottom = newBottom;
		}
	}
	return true;
}

void Orthographic::resetCamera() {
	left = iLeft;
	right = iRight;
	top = iTop;
	bottom = iBottom;

	rotation[0] = 0;
	rotation[1] = 0;
	rotation[2] = 0;

}

float distanceBetweenPoints(float point1[3], float point2[3]) {
	return sqrt((point2[0]-point1[0])*(point2[0]-point1[0]) + (point2[1]-point1[1])*(point2[1]-point1[1]) + (point2[2]-point1[2])*(point2[2]-point1[2]));
}

float radians(float degrees) {
	return degrees * ((float) M_PI / 180.f);
}

float degrees(float radians) {
	return radians / ((float) M_PI / 180.f);
}