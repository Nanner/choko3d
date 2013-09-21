#include "YAFTexture.h"

YAFTexture::YAFTexture(string id, string file) {
	this->id = id;
	this->file = file;

	validate();
}

void YAFTexture::validate() {

	ifstream textFile(file.c_str());
	if(textFile.is_open()) {
		textFile.close();
	}
	else
		throw(InvalidAttributeValueException("Texture: file (Texture file doesn't exist?)"));
}
