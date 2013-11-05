#include "ShaderTexture.h"

ShaderTexture::ShaderTexture(string t): CGFtexture(t) {
	Appearance::currentTextIndex++;
}

unsigned int ShaderTexture::getTexID() {return texID;}