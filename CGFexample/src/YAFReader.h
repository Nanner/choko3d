#ifndef _YAFReader_H_
#define _YAFReader_H_

#include "tinyxml.h"
#include "CGFscene.h"
#include "YAFGlobal.h"
#include "YAFCamera.h"
#include "YAFGlobalLighting.h"
#include "YAFLight.h"
#include "YAFTexture.h"
#include "YAFAppearance.h"
#include "YAFNode.h"
#include "Transformation.h"
#include "ScenePrimitive.h"
#include "Plane.h"
#include "Patch.h"
#include "Animation.h"
#include "LinearAnimation.h"
#include "Waterline.h"
#include "Vehicle.h"

#include <sstream>
#include <iostream>
#include <map>

class YAFReader
{
public:
    YAFReader(char *filename);
    ~YAFReader();

    static TiXmlElement *findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val);
    template<class T> vector<T> getValues(TiXmlElement * element, vector<string> attributeNames);
    template<class T> T getValue(TiXmlElement * element, char * attributeName);
    template<class T> vector<T> getValues(TiXmlElement * element, char * attributeName);

	YAFGlobal globals;
    YAFGlobalLighting globalLighting;
	map<string, YAFCamera> cameras;
    map<string, YAFLight> lights;
	map<string, YAFTexture> textures;
	map<string, YAFAppearance> appearances;
    map<string, YAFNode> nodes;
    map<string, Animation *> animations;
	map<string, YAFNode> pickingSquares;
	map<string, YAFNode> boardPieces;

    
protected:

    TiXmlDocument* doc;

    TiXmlElement* globalsElement; 
    TiXmlElement* camerasElement;
    TiXmlElement* lightingElement;
    TiXmlElement* texturesElement;
    TiXmlElement* appearancesElement;
    TiXmlElement* graphElement;
    TiXmlElement* animationsElement;
	TiXmlElement* boardPiecesElement;

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
        if ( ss >> value ) {
            values.push_back(value);
        }
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