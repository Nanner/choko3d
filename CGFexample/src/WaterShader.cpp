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

	//Get the uniform location for the scales and set the default values
	speedScaleLoc = glGetUniformLocation(id(), "speedScale");
	glUniform1f(speedScaleLoc, DEFAULT_SPEED);
	
	heightScaleLoc = glGetUniformLocation(id(), "heightScale");
	glUniform1f(heightScaleLoc, DEFAULT_HEIGHT);

	inclineScaleLoc = glGetUniformLocation(id(), "inclineScale");
	glUniform1f(inclineScaleLoc, DEFAULT_INCLINE);

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

	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	//Activate texture unit and apply it
	glActiveTexture(GL_TEXTURE0 + waterTextureIndex);
	waterTexture->apply();

	//Same
	glActiveTexture(GL_TEXTURE0 + heightMapIndex);
	heightMap->apply();

}

void WaterShader::unbind(void) {
	CGFshader::unbind();

	//Deactivate texture
	glActiveTexture(GL_TEXTURE0);
}

void WaterShader::setScalesFromControlValues(int sControl, int hControl, int iControl) {
	float speedScale = (float) (sControl * DEFAULT_SPEED) / 50.0;
	float heightScale = (float) (hControl * DEFAULT_HEIGHT) / 50.0;
	float inclineScale = (float) (iControl * DEFAULT_INCLINE) / 50.0;

	bind();
	glUniform1f(speedScaleLoc, speedScale);
	glUniform1f(heightScaleLoc, heightScale);
	glUniform1f(inclineScaleLoc, inclineScale);
	unbind();
}

WaterShader::~WaterShader(void) {
	delete heightMap;
    delete waterTexture;
}