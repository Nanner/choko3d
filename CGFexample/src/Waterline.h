#ifndef WATERLINE_H
#define WATERLINE_H

#include "ScenePrimitive.h"
#include "WaterShader.h"
#include "Plane.h"

#define WATERLINE_EVAL_W_PARTS 30
#define WATERLINE_EVAL_H_PARTS 300


class Waterline : public ScenePrimitive {

protected:
	WaterShader * shader;

	static GLfloat ctrlpoints[10][3];
	static GLfloat nrmlcompon[10][3];
	static GLfloat texels[10][2];
	GLint previousFrontFace;

    
public:
    string heightmap;
    string texturemap;
    string fragmentshader;
    string vertexshader;
    
    Waterline(string heightmap, string texturemap, string fragmentshader, string vertexshader);
	void draw();
	void drawEval();
	WaterShader* getShader();

};

#endif
