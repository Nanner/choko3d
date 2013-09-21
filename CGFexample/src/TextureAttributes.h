#ifndef TEXTUREATTRIBUTES_H
#define TEXTUREATTRIBUTES_H

#include "YAFElement.h"

class TextureAttributes: public YAFElement {
private:
	static vector<string> usedTextureIDs;

public:
	string id;
	string file;

	TextureAttributes(string id, string file);
	void validate();
};

#endif