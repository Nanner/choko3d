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

#define DEFAULT_SPEED 0.00005
#define DEFAULT_HEIGHT 1.5
#define DEFAULT_INCLINE 0.05


using namespace std;

class WaterShader : public CGFshader {
    
public:
    WaterShader(string heightmap, string texturemap, string fragmentshader, string vertexshader);
    virtual void bind(void);
	virtual void unbind(void);
	//This method allows setting the scale values of speed, height and incline according to control values
	//ranging from -500 to 500 for speed, 0 to 100 for height and -100 to 100 for incline
	void setScalesFromControlValues(int sControl, int hControl, int iControl);

    ~WaterShader();
    
protected:
	unsigned int waterTextureIndex;
	unsigned int heightMapIndex;
	ShaderTexture * waterTexture;
	ShaderTexture * heightMap;
    
	GLint speedScaleLoc;
	GLint heightScaleLoc;
	GLint inclineScaleLoc;

	GLint heightImageLoc;
	GLint waterImageLoc;
};

#endif
