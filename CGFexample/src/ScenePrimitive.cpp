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
	glNormal3f(0,0,1);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(xy1[0], xy1[1], 0);
		glTexCoord2f(1, 0);
		glVertex3f(xy2[0], xy1[1], 0);
		glTexCoord2f(1, 1);
		glVertex3f(xy2[0], xy2[1], 0);
		glTexCoord2f(0, 1);
		glVertex3f(xy1[0], xy2[1], 0);
	glEnd();
}

bool Rectangle::operator==( const Rectangle &r2 ) const {
	return (this->xy1[0] == r2.xy1[0]) && (this->xy1[1] == r2.xy1[1]) &&
		(this->xy2[0] == r2.xy2[0]) && (this->xy2[1] == r2.xy2[1]);
}

bool Rectangle::isSamePrimitive( const ScenePrimitive &p2 ) const {
	const Rectangle* rect2 = dynamic_cast< const Rectangle* >( &p2 );
	return rect2 != NULL && *this == *rect2;
}


Triangle::Triangle(vector<float> xyz1, vector<float> xyz2, vector<float> xyz3) {
	matrix = NULL;
    for (int i = 0; i < 3; i++) {
        this->xyz1[i] = xyz1.at(i);
        this->xyz2[i] = xyz2.at(i);
        this->xyz3[i] = xyz3.at(i);
    }
	//todo apply texture mapping to triangle and rectangle. We need to be careful with the s and t's and apply the text coords correctly
}

void Triangle::render() {
	glBegin(GL_TRIANGLES);
		glVertex3fv(xyz1);
		glVertex3fv(xyz2);
		glVertex3fv(xyz3);
	glEnd();
}

bool Triangle::operator==( const Triangle &t2 ) const {
	return (this->xyz1[0] == t2.xyz1[0]) && (this->xyz1[1] == t2.xyz1[1]) && (this->xyz1[2] == t2.xyz1[2]) &&
		(this->xyz2[0] == t2.xyz2[0]) && (this->xyz2[1] == t2.xyz2[1]) && (this->xyz2[2] == t2.xyz2[2]) &&
		(this->xyz3[0] == t2.xyz3[0]) && (this->xyz3[1] == t2.xyz3[1]) && (this->xyz3[2] == t2.xyz3[2]);
}

bool Triangle::isSamePrimitive( const ScenePrimitive &p2 ) const {
	const Triangle* triangle2 = dynamic_cast< const Triangle* >( &p2 );
	return triangle2 != NULL && *this == *triangle2;
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

	quadratic = gluNewQuadric();
	gluQuadricTexture(quadratic, GL_TRUE);
}

void Cylinder::render() {
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

bool Cylinder::operator==( const Cylinder &c2 ) const {
	return (this->base == c2.base) && (this->top == c2.top) && (this->height == c2.height) && (this->slices == c2.slices) && (this->stacks == c2.stacks);
}

bool Cylinder::isSamePrimitive( const ScenePrimitive &p2 ) const {
	const Cylinder* cyl2 = dynamic_cast< const Cylinder* >( &p2 );
	return cyl2 != NULL && *this == *cyl2;
}


Sphere::Sphere(float radius, int slices, int stacks) {
	matrix = NULL;
    this->radius = radius;
    this->slices = slices;
    this->stacks = stacks;

	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
}

void Sphere::render(){
	glPushMatrix();

	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glutSolidSphere(radius, slices, stacks);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glPopMatrix();
}

bool Sphere::operator==( const Sphere &s2 ) const {
	return (this->radius == s2.radius) && (this->slices == s2.slices) && (this->stacks == s2.stacks);
}

bool Sphere::isSamePrimitive( const ScenePrimitive &p2 ) const {
	const Sphere* sph2 = dynamic_cast< const Sphere* >( &p2 );
	return sph2 != NULL && *this == *sph2;
}


Torus::Torus(float inner, float outer, int slices, int loops) {
	matrix = NULL;
    this->inner = inner;
    this->outer = outer;
    this->slices = slices;
    this->loops = loops;

	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
}

void Torus::render(){
	glPushMatrix();

	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glutSolidTorus(inner, outer, slices, loops);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glPopMatrix();
}

bool Torus::operator==( const Torus &t2 ) const {
	return (this->inner == t2.inner) && (this->outer == t2.outer) && (this->slices == t2.slices) && (this->loops == t2.loops);
}

bool Torus::isSamePrimitive( const ScenePrimitive &p2 ) const {
	const Torus* tor2 = dynamic_cast< const Torus* >( &p2 );
	return tor2 != NULL && *this == *tor2;
}

