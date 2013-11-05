#include "Waterline.h"

Waterline::Waterline(string heightmap, string texturemap, string fragmentshader, string vertexshader) {
    matrix = NULL;
    this->shader = new WaterShader(heightmap, texturemap, fragmentshader, vertexshader);
    this->plane1 = new Plane(90);
	this->plane2 = new Plane(90);
	this->plane3 = new Plane(90);
	this->plane4 = new Plane(90);
    plane1->setAppearance(NULL);
    plane1->setAnimation(NULL);
	plane2->setAppearance(NULL);
	plane2->setAnimation(NULL);
	plane3->setAppearance(NULL);
	plane3->setAnimation(NULL);
	plane4->setAppearance(NULL);
	plane4->setAnimation(NULL);
}

void Waterline::draw() {
	glPushMatrix();
    shader->bind();
    plane1->draw();
	glTranslatef(0, 0, 1);
	plane2->draw();
	glTranslatef(0, 0, 1);
	plane3->draw();
	glTranslatef(0, 0, 1);
	plane4->draw();
    shader->unbind();
	glPopMatrix();
}

CGFshader* Waterline::getShader() {
	return shader;
}
