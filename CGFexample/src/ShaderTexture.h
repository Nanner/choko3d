#ifndef SHADERTEXTURE_H
#define SHADERTEXTURE_H

#include "CGFtexture.h"
#include "Appearance.h"

class ShaderTexture : public CGFtexture {
public:
	ShaderTexture(string);
	unsigned int getTexID();
};

#endif
