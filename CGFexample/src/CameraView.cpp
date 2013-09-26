#include "CameraView.h"

CameraView::CameraView(string id, float near, float far): id(id), near(near), far(far) {
	
	//Calculate the aspect ratio of the scene/app viewport
	GLint m_viewport[4];
	glGetIntegerv( GL_VIEWPORT, m_viewport );
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
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Load the camera view projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(angle, aspect, near, far);

	//Load the camera position related matrix transformations
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	float upVector[3] = {0, 0, 0};
	if(target[0] == position[0] && target[2] == position[2])
		upVector[0] = 1;
	else
		upVector[1] = 1;

	gluLookAt(position[0], position[1], position[2], target[0], target[1], target[2], upVector[0], upVector[1], upVector[2]);
}

Orthographic::Orthographic(string id, float near, float far, float left, float right, float top, float bottom):
	CameraView(id, near, far), left(left), right(right), top(top), bottom(bottom) {}

void Orthographic::applyView() {
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Same as above, projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho(left*aspect, right*aspect, bottom, top, near, far);

	//Same as above, model matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}