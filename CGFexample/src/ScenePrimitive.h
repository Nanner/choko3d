#ifndef _SCENEPRIMITIVE_H
#define _SCENEPRIMITIVE_H

#include "CGFobject.h"
#include "SceneVertex.h"


class ScenePrimitive: public SceneVertex {
public:
	virtual void render() = 0;
	virtual bool isSamePrimitive(const ScenePrimitive &p2) const = 0;
};

class Rectangle: public ScenePrimitive {
public:
	float xy1[2];
	float xy2[2];

	Rectangle(/*needs mats and texts and transforms*/vector<float> xy1, vector<float> xy2);
	Rectangle(/*same*/float x1, float y1, float x2, float y2);

	void render();

	bool operator==(const Rectangle &r2) const;

	virtual bool isSamePrimitive( const ScenePrimitive &p2 ) const;

};

class Triangle : public ScenePrimitive {
public:
    float xyz1[3];
    float xyz2[3];
    float xyz3[3];
    
    Triangle(vector<float> xyz1, vector<float> xyz2, vector<float> xyz3);
    void render();

	bool operator==(const Triangle &t2) const;

	virtual bool isSamePrimitive( const ScenePrimitive &p2 ) const;

};

class Cylinder : public ScenePrimitive {
public:
    float base, top, height, slices, stacks;
    
    Cylinder(float base,
             float top,
             float height,
             float slices,
             float stacks);
    void render();

	bool operator==(const Cylinder &c2) const;

	virtual bool isSamePrimitive( const ScenePrimitive &p2 ) const;

};

class Sphere : public ScenePrimitive {
public:
    float radius;
    int slices, stacks;
    
    Sphere(float radius, int slices, int stacks);
    void render();

	bool operator==(const Sphere &s2) const;

	virtual bool isSamePrimitive( const ScenePrimitive &p2 ) const;

};

class Torus : public ScenePrimitive {
public:
    float inner, outer;
    int slices, loops;
    
    Torus(float inner, float outer, int slices, int loops);
    void render();

	bool operator==(const Torus &t2) const;

	virtual bool isSamePrimitive( const ScenePrimitive &p2 ) const;

};

#endif