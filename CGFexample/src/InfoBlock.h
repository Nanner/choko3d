#ifndef INFOBLOCK_H
#define INFOBLOCK_H

#define RGBA_LOWER_BOUND 0.0
#define RGBA_UPPER_BOUND 1.0

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

class InfoBlock {
public:
	virtual void validate() = 0;
};

class InvalidAttributeValueException {
private:
	string failedAttribute;

public:
	InvalidAttributeValueException(string failedAttribute) {
		this->failedAttribute = failedAttribute;
	}

	string what() const {
		string ret = "Invalid value for attribute '" + failedAttribute + "'!";
		return ret.c_str();
	}
};

class EmptyAttributeException {
private:
	string failedAttribute;

public:
	EmptyAttributeException(string failedAttribute) {
		this->failedAttribute = failedAttribute;
	}

	string what() const {
		string ret = "The attribute '" + failedAttribute + "' is empty!";
		return ret.c_str();
	}
};

#endif