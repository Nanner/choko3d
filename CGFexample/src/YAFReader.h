#ifndef _YAFReader_H_
#define _YAFReader_H_

#include "tinyxml.h"
#include "CGFscene.h"
#include "YAFGlobal.h"
#include "YAFCamera.h"
#include <sstream>
#include <iostream>
#include <map>

class YAFReader : public CGFscene 
{
public:
    YAFReader(char *filename);
    ~YAFReader();

    static TiXmlElement *findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val);
    template<class T> vector<T> getValues(TiXmlElement * element, vector<string> attributeNames);
    template<class T> T getValue(TiXmlElement * element, char * attributeName);
    template<class T> vector<T> getValues(TiXmlElement * element, char * attributeName);

	YAFGlobal globals;
	map<string, YAFCamera> cameras;

    
protected:

    TiXmlDocument* doc;

    TiXmlElement* globalsElement; 
    TiXmlElement* camerasElement;
    TiXmlElement* lightingElement;
    TiXmlElement* texturesElement;
    TiXmlElement* appearancesElement;
    TiXmlElement* graphElement;

};

template<class T>
vector<T> YAFReader::getValues(TiXmlElement * element, char * attributeName) {
    vector<T> values;
    T value;
    
    char* valString = NULL;
    valString = (char *) element->Attribute(attributeName);
    
    if (valString == NULL || (strcmp(valString, "") == 0))
		throw(EmptyAttributeException(attributeName));
    
    stringstream ss (valString);
    ss << boolalpha;
    
    while ( !ss.eof() ) {
        if ( ss >> value )
            values.push_back(value);
        else {
            throw(InvalidAttributeValueException(attributeName));
            break;
        }
    }
    
    return values;
}

template<class T>
T YAFReader::getValue(TiXmlElement * element, char * attributeName) {
    T value;

    vector<T> values = getValues<T>(element, attributeName);
    if (values.size() > 0 )
        return values.at(0);
    else
        return value;
}


template<class T>
vector<T> YAFReader::getValues(TiXmlElement* element, vector<string> attributeNames) {
    
    vector<T> values;
    
    for(size_t i = 0; i < attributeNames.size(); i++) {
        
        vector<T> currentValues = getValues<T>( element, (char*)attributeNames.at(i).c_str() );
        
        for(size_t j = 0; j < currentValues.size(); j++ ){
            values.push_back( currentValues.at(j) );
        }
    }
    
    return values;
}


#endif