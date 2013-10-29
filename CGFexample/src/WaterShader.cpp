#include "WaterShader.h"

WaterShader::WaterShader(string heightmap, string texturemap, string fragmentshader, string vertexshader)
{
    init(vertexshader.c_str(), fragmentshader.c_str());
    
    CGFshader::bind();
    
    
}

void WaterShader::bind(void) {
    CGFshader::bind();
}



WaterShader::~WaterShader(void)
{
	delete heightTexture;
    delete texture;
}