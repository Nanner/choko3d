#ifndef _SCENEPRIMITIVE_H
#define _SCENEPRIMITIVE_H

#include "CGFobject.h"
#include "SceneVertex.h"


class ScenePrimitive: public SceneVertex {
public:
	virtual void render() = 0;
};

class Rectangle: public ScenePrimitive {
public:
	float xy1[2];
	float xy2[2];

	Rectangle(/*needs mats and texts and transforms*/float xy1[2], float xy2[2]);
	Rectangle(/*same*/float x1, float y1, float x2, float y2);

	void render();
};

#endif