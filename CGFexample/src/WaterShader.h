#ifndef WATERSHADER_H
#define WATERSHADER_H

#include "CGFshader.h"
#include "CGFtexture.h"
#include <string>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif


using namespace std;

class WaterShader : public CGFshader {
    
public:
    WaterShader(string heightmap, string texturemap, string fragmentshader, string vertexshader);
    virtual void bind(void);
    ~WaterShader();
    
protected:
	CGFtexture * heightTexture;
	CGFtexture * texture;
    
	GLint heightImageLoc;
	GLint imageLoc;
};

#endif
