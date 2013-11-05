#ifndef WATERSHADER_H
#define WATERSHADER_H

#include "CGFshader.h"
#include "ShaderTexture.h"
#include "Appearance.h"
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
	virtual void unbind(void);
    ~WaterShader();
    
protected:
	unsigned int waterTextureIndex;
	unsigned int heightMapIndex;
	ShaderTexture * waterTexture;
	ShaderTexture * heightMap;
    
	GLint heightImageLoc;
	GLint waterImageLoc;
};

#endif
