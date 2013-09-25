#include "ScenePrimitive.h"
#include <iostream>


Rectangle::Rectangle(vector<float> xy1, vector<float> xy2) {
	matrix = NULL;
    for (int i = 0; i < 2; i++) {
        this->xy1[i] = xy1.at(i);
        this->xy2[i] = xy2.at(i);
    }
}

Rectangle::Rectangle(float x1, float y1, float x2, float y2) {
	matrix = NULL;
	xy1[0] = x1;
	xy1[1] = y1;

	xy2[0] = x2;
	xy2[1] = y2;
}

void Rectangle::render() {
	glRectf(xy1[0], xy1[1], xy2[0], xy2[1]);
}


Triangle::Triangle(vector<float> xyz1, vector<float> xyz2, vector<float> xyz3) {
	matrix = NULL;
    for (int i = 0; i < 3; i++) {
        this->xyz1[i] = xyz1.at(i);
        this->xyz2[i] = xyz2.at(i);
        this->xyz3[i] = xyz3.at(i);
    }
}

void Triangle::render() {
	glBegin(GL_TRIANGLES);
		glVertex3fv(xyz1);
		glVertex3fv(xyz2);
		glVertex3fv(xyz3);
	glEnd();
}

Cylinder::Cylinder(float base,
                   float top,
                   float height,
                   float slices,
                   float stacks) {
	matrix = NULL;
    this->base = base;
    this->top = top;
    this->height = height;
    this->slices = slices;
    this->stacks = stacks;
}

void Cylinder::render(){
	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	gluCylinder(quadratic, base, top, height, slices, stacks);
	glPushMatrix();
	glRotatef(180.0, 0.0, 1.0, 0.0);
	gluDisk(quadratic, 0, base, slices, stacks);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.0, height);
	gluDisk(quadratic, 0.0, top, slices, stacks);
	glPopMatrix();
}


Sphere::Sphere(float radius, int slices, int stacks) {
	matrix = NULL;
    this->radius = radius;
    this->slices = slices;
    this->stacks = stacks;
}

void Sphere::render(){
	glutSolidSphere(radius, slices, stacks);
}


Torus::Torus(float inner, float outter, int slices, int loops) {
	matrix = NULL;
    this->inner = inner;
    this->outer = outter;
    this->slices = slices;
    this->loops = loops;
}

void Torus::render(){
	glutSolidTorus(inner, outer, slices, loops);
}

