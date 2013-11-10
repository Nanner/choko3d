#ifndef INFOBLOCK_H
#define INFOBLOCK_H

#define RGBA_MIN 0.0
#define RGBA_MAX 1.0

#define SHININESS_MIN 0.0
#define SHININESS_MAX 128.0

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>

#define ERROR_LEN 100

using namespace std;

class YAFElement {
public:
    static bool validRGBA(float r, float g, float b, float a);
    static bool validXYZ(float x, float y, float z);
    static bool validAngle(float angle);
    static bool validMinMax(float min, float max);
	static bool validShininess(float shininess);

private:
	virtual void validate() = 0;
};

class InvalidAttributeValueException {
private:
	string failedAttribute;

public:
	InvalidAttributeValueException(string failedAttribute) {
		this->failedAttribute = failedAttribute;
	}

	string error() const {
		string ret = "Invalid value for attribute '" + failedAttribute + "'!";
		return ret;
	}
};

class EmptyAttributeException {
private:
	string failedAttribute;

public:
	EmptyAttributeException(string failedAttribute) {
		this->failedAttribute = failedAttribute;
	}

	string error() const {
		string ret = "The attribute '" + failedAttribute + "' is empty!";
		return ret;
	}
};

#endif