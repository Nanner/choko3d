#include "CameraView.h"

CameraView::CameraView(string id, float near, float far): id(id), near(near), far(far) {
	
	//Calculate the aspect ratio of the scene/app viewport
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	aspect = (float) m_viewport[2] / (float) m_viewport[3];
}

Perspective::Perspective(string id, float near, float far, float angle, float posX, float posY, float posZ, float targetX, float targetY, float targetZ):
	CameraView(id, near, far), angle(angle) {
		position[0] = posX;
		position[1] = posY;
		position[2] = posZ;

		target[0] = targetX;
		target[1] = targetY;
		target[2] = targetZ;
}

void Perspective::applyView() {

	//Load the camera view projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle, aspect, near, far);

	//Load the camera position related matrix transformations
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float upVector[3] = {0, 0, 0};
	if(target[0] == position[0] && target[2] == position[2])
		upVector[0] = 1;
	else
		upVector[1] = 1;

	gluLookAt(position[0], position[1], position[2], target[0], target[1], target[2], upVector[0], upVector[1], upVector[2]);
}

bool Perspective::translate(int axis, float value) {
	if (axis!=CG_CGFcamera_AXIS_X && axis!=CG_CGFcamera_AXIS_Y && axis!=CG_CGFcamera_AXIS_Z) return false;

	//If the movement is in the Z axis, means we want to zoom in on the scene
	if(axis == CG_CGFcamera_AXIS_Z) {
		float newAngle = angle + 2 * value;
		if(newAngle > 0 && newAngle < 90)
			angle = newAngle;
	}


	return true;
}

Orthographic::Orthographic(string id, float near, float far, float left, float right, float top, float bottom):
	CameraView(id, near, far), left(left), right(right), top(top), bottom(bottom) {}

void Orthographic::applyView() {

	//Same as above, projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left*aspect, right*aspect, bottom, top, near, far);

	//Same as above, model matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0,-1.0, 0.0, 1.0, 0.0);
	glRotatef(rotation[0], 1.f, 0.f, 0.f);
	glRotatef(rotation[1], 0.f, 1.f, 0.f);
	glRotatef(rotation[2], 0.f, 0.f, 1.f);
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