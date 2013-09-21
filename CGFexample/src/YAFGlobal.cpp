#include "YAFGlobal.h"

YAFGlobal::YAFGlobal( vector<float> rgbaValues, vector<string> backgroundAttributes )
{
	red = rgbaValues.at(0);
	green = rgbaValues.at(1);
	blue = rgbaValues.at(2);
	alpha = rgbaValues.at(3);

	drawmode = backgroundAttributes.at(0);
	shading = backgroundAttributes.at(1);
	cullface = backgroundAttributes.at(2);
	cullorder = backgroundAttributes.at(3);

	try {
		validate();
	}
	catch (InvalidAttributeValueException &ive) {
        throw;
	}

}

void YAFGlobal::validate() {
    
    if ( !validRGBA(red, green, blue, alpha) ) {
        throw(InvalidAttributeValueException("background (RGBA)"));
    }

	vector<string> drawmodeOptions;
	drawmodeOptions.push_back("fill");
	drawmodeOptions.push_back("line");
	drawmodeOptions.push_back("point");

	if(find(drawmodeOptions.begin(), drawmodeOptions.end(), drawmode) == drawmodeOptions.end()) {
		throw(InvalidAttributeValueException("drawmode"));
	}

	vector<string> shadingOptions;
	shadingOptions.push_back("flat");
	shadingOptions.push_back("gouraud");

	if(find(shadingOptions.begin(), shadingOptions.end(), shading) == shadingOptions.end()) {
		throw(InvalidAttributeValueException("shading"));
	}

	vector<string> cullfaceOptions;
	cullfaceOptions.push_back("none");
	cullfaceOptions.push_back("back");
	cullfaceOptions.push_back("front");
	cullfaceOptions.push_back("both");

	if(find(cullfaceOptions.begin(), cullfaceOptions.end(), cullface) == cullfaceOptions.end()) {
		throw(InvalidAttributeValueException("cullface"));
	}

	vector<string> cullorderOptions;
	cullorderOptions.push_back("CCW");
	cullorderOptions.push_back("CW");

	if(find(cullorderOptions.begin(), cullorderOptions.end(), cullorder) == cullorderOptions.end()) {
		throw(InvalidAttributeValueException("cullorder"));
	}


}
