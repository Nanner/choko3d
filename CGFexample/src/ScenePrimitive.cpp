#include "ScenePrimitive.h"
#include <iostream>

Rectangle::Rectangle(float xy1[2], float xy2[2]) {
	matrix = NULL;
	this->xy1[0] = xy1[0];
	this->xy1[1] = xy1[1];

	this->xy2[0] = xy2[0];
	this->xy2[1] = xy2[1];
}

Rectangle::Rectangle(float x1, float y1, float x2, float y2) {
	matrix = NULL;
	xy1[0] = x1;
	xy1[1] = y1;

	xy2[0] = x2;
	xy2[1] = y2;
}

void Rectangle::render() {
	//glPushMatrix(); //<-- maybe not needed due to dfs-own push and pop?
	/* apply transforms and stuff */
	cout << "Drawing random rectangle!" << xy1[0] << xy1[1] << xy2[0] << xy2[1] << endl;
	glRectf(xy1[0], xy1[1], xy2[0], xy2[1]);
	//glPopMatrix();
}