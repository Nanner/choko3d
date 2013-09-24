#include "ScenePrimitive.h"
#include <iostream>

Rectangle::Rectangle(vector<float> xy1, vector<float> xy2) {
    for (int i = 0; i < 2; i++) {
        this->xy1[i] = xy1.at(i);
        this->xy2[i] = xy2.at(i);
    }
}

Rectangle::Rectangle(float x1, float y1, float x2, float y2) {
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


Triangle::Triangle(vector<float> xyz1, vector<float> xyz2, vector<float> xyz3) {
    for (int i = 0; i < 3; i++) {
        this->xyz1[i] = xyz1.at(i);
        this->xyz2[i] = xyz2.at(i);
        this->xyz3[i] = xyz3.at(i);
    }
}

void Triangle::render(){}

Cylinder::Cylinder(float base,
                   float top,
                   float height,
                   float slices,
                   float stacks) {
    this->base = base;
    this->top = top;
    this->height = height;
    this->slices = slices;
    this->stacks = stacks;
}

void Cylinder::render(){}

Sphere::Sphere(float radius, int slices, int stacks) {
    this->radius = radius;
    this->slices = slices;
    this->stacks = stacks;
}

void Sphere::render(){}

Torus::Torus(float inner, float outter, int slices, int loops) {
    this->inner = inner;
    this->outter = outter;
    this->slices = slices;
    this->loops = loops;
}

void Torus::render(){}