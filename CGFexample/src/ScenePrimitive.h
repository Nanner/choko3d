#ifndef _SCENEPRIMITIVE_H
#define _SCENEPRIMITIVE_H

#include "CGFobject.h"
#include "SceneVertex.h"
#include "CGFshader.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <math.h>

#define MARGIN 0.01

class ScenePrimitive: public SceneVertex {
public:
	virtual void draw() = 0;
};

namespace Primitives {
	class Rectangle: public ScenePrimitive {
	public:
		float x1, y1;
		float x2, y2;

		static int rows;
		static int columns;

		float xScaled, yScaled, distX, distY;
		float deltaX, deltaY, texDeltaX, texDeltaY;

		Rectangle(vector<float> xy1, vector<float> xy2);

		void draw();

	};
}

class Triangle : public ScenePrimitive {
public:
	float x1, y1, z1;
	float x2, y2, z2;
	float x3, y3, z3;
	float xn, yn, zn;

	float texelBs, texelCs, texelCt;

	bool texelsReady;

	Triangle(vector<float> xyz1, vector<float> xyz2, vector<float> xyz3);
	void draw();

	void calculateNormal();

	void calculateTexels();

};

class Cylinder : public ScenePrimitive {
public:
	GLUquadricObj *quadratic;
	float base, top, height, slices, stacks;

	Cylinder(float base,
		float top,
		float height,
		float slices,
		float stacks);
	void draw();

};

class Sphere : public ScenePrimitive {
public:
	float radius;
	int slices, stacks;
	GLUquadric *quad;

	Sphere(float radius, int slices, int stacks);
	void draw();

};

class Torus : public ScenePrimitive {
public:
	float inner, outer;
	int slices, loops;

	Torus(float inner, float outer, int slices, int loops);
	void draw();

};
bool reachedEnd(float start, float end, float direction) ;

#endif