//For example, for glGetUniformLocation
#include <GL/glew.h>

#include "WaterShader.h"

WaterShader::WaterShader(string heightmap, string texturemap, string fragmentshader, string vertexshader) {
	init(vertexshader.c_str(), fragmentshader.c_str());

	//Activate shader
	CGFshader::bind();

	//Load associated texture/height maps
	heightMapIndex = Appearance::currentTextIndex;
	heightMap = new ShaderTexture(heightmap.c_str());

	waterTextureIndex = Appearance::currentTextIndex;
	waterTexture = new ShaderTexture(texturemap.c_str());

	

	//Get the uniform location for the water texture sampler and set the associated texture
	waterImageLoc = glGetUniformLocation(id(), "waterImage");
	glUniform1i(waterImageLoc, waterTextureIndex);

	//Get the uniform location for the height map sampler and set the associated texture
	heightImageLoc = glGetUniformLocation(id(), "heightImage");
	glUniform1i(heightImageLoc, heightMapIndex);
	
	//Deactivate shader
	CGFshader::unbind();
}

void WaterShader::bind(void) {
    CGFshader::bind();

	//Activate texture unit and apply it
	glActiveTexture(GL_TEXTURE0 + waterTextureIndex);
	waterTexture->apply();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Same
	glActiveTexture(GL_TEXTURE0 + heightMapIndex);
	heightMap->apply();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void WaterShader::unbind(void) {
	CGFshader::unbind();

	//Deactivate texture
	glActiveTexture(GL_TEXTURE0);
}



WaterShader::~WaterShader(void) {
	delete heightMap;
    delete waterTexture;
}