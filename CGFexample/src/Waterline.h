#ifndef WATERLINE_H
#define WATERLINE_H

#include "ScenePrimitive.h"
#include "WaterShader.h"
#include "Plane.h"

class Waterline : public ScenePrimitive {

protected:
	WaterShader * shader;
    Plane * plane;

    
public:
    string heightmap;
    string texturemap;
    string fragmentshader;
    string vertexshader;
    
    Waterline(string heightmap, string texturemap, string fragmentshader, string vertexshader);
	void draw();
	WaterShader* getShader();

};

#endif
