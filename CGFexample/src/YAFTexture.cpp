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
	else {
        char error[ERROR_LEN];
        sprintf (error,"Texture: file (Texture '%s' doesn't exist?)", file.c_str() );
		throw(InvalidAttributeValueException(error));
    }
}
