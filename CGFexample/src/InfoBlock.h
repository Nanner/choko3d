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
	virtual void validate();
};

class InvalidAttributeValueException: public exception {
private:
	string failedAttribute;

public:
	InvalidAttributeValueException(string failedAttribute) {
		this->failedAttribute = failedAttribute;
	}

	virtual const char* what() const throw()
	{
		string ret = "Invalid value for attribute " + failedAttribute + "!";
		return ret.c_str();
	}
};

#endif