#ifndef YAFTEXTURE_H
#define YAFTEXTURE_H

#include "YAFElement.h"

class YAFTexture: public YAFElement {
private:

public:
	string id;
	string file;

	YAFTexture(string id, string file);
	void validate();
};

#endif