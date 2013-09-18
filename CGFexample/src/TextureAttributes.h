#ifndef TEXTUREATTRIBUTES_H
#define TEXTUREATTRIBUTES_H

#include "InfoBlock.h"

class TextureAttributes: public InfoBlock {
private:
	static vector<string> usedTextureIDs;

public:
	string id;
	string file;

	TextureAttributes(string id, string file);
	void validate();
};

#endif