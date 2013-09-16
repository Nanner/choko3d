#include "XMLScene.h"

XMLScene::XMLScene(char *filename)
{

	// Read XML from file

	doc=new TiXmlDocument( filename );
	bool loadOkay = doc->LoadFile();

	if ( !loadOkay )
	{
		printf( "Could not load file '%s'. Error='%s'. Exiting.\n", filename, doc->ErrorDesc() );
		exit( 1 );
	}

	TiXmlElement* yafElement= doc->FirstChildElement( "yaf" );

	if (yafElement == NULL)
	{
		printf("Main yaf block element not found! Exiting!\n");
		exit(1);
	}

	globalsElement = yafElement->FirstChildElement( "globals" );
	camerasElement = yafElement->FirstChildElement( "cameras" );
	lightingElement =  yafElement->FirstChildElement( "lighting" );
	texturesElement =  yafElement->FirstChildElement( "textures" );
	appearancesElement =  yafElement->FirstChildElement( "appearances" );

	graphElement =  yafElement->FirstChildElement( "graph" );


	// Init
	// An example of well-known, required nodes

	if (globalsElement == NULL)
		printf("Globals block not found!\n");
	else
	{
		printf("Processing globals:\n");

		vector<float> rgba = getFloatValues(globalsElement, (char*) "background");

        printf("  background values (RGBA): %f %f %f %f\n", rgba.at(0), rgba
               .at(1), rgba.at(2), rgba.at(3));
        // TODO store rgba values
        
        vector<string> bgAttributesNames;
        bgAttributesNames.push_back("drawmode"); bgAttributesNames.push_back("shading");
        bgAttributesNames.push_back("cullface"); bgAttributesNames.push_back("cullorder");

		vector<string> backgroundAttributes = getStringValues(globalsElement, bgAttributesNames);
        // TODO store background attributes

		TiXmlElement* backgroundElement=globalsElement->FirstChildElement("frustum");
		if (backgroundElement)
		{
			float near,far;

			if (backgroundElement->QueryFloatAttribute("near",&near)==TIXML_SUCCESS && 
				backgroundElement->QueryFloatAttribute("far",&far)==TIXML_SUCCESS
				)
				printf("  frustum attributes: %f %f\n", near, far);
			else
				printf("Error parsing frustum\n");
		}
		else
			printf("frustum not found\n");


		// translate: example of a node with an attribute comprising several float values
		// It shows an example of extracting an attribute's value, and then further parsing that value 
		// to extract individual values
		TiXmlElement* translateElement=globalsElement->FirstChildElement("translate");
		if (translateElement)
		{
			char *valString=NULL;
			float x,y,z;

			valString=(char *) translateElement->Attribute("xyz");

			if(valString && sscanf(valString,"%f %f %f",&x, &y, &z)==3)
			{
				printf("  translate values (XYZ): %f %f %f\n", x, y, z);
			}
			else
				printf("Error parsing translate");
		}
		else
			printf("translate not found\n");		

		// repeat for each of the variables as needed
	}

	// Other blocks could be validated/processed here


	// graph section
	if (graphElement == NULL)
		printf("Graph block not found!\n");
	else
	{
		//char *prefix="  -";
		TiXmlElement *node=graphElement->FirstChildElement();

		while (node)
		{
			printf("Node id '%s' - Descendants:\n",node->Attribute("id"));
			TiXmlElement *child=node->FirstChildElement();
			while (child)
			{
				if (strcmp(child->Value(),"Node")==0)
				{
					// access node data by searching for its id in the nodes section
					
					TiXmlElement *noderef=findChildByAttribute(appearancesElement,"id",child->Attribute("id"));

					if (noderef)
					{
						// print id
						printf("  - Node id: '%s'\n", child->Attribute("id"));

						// prints some of the data
						printf("    - Material id: '%s' \n", noderef->FirstChildElement("material")->Attribute("id"));
						printf("    - Texture id: '%s' \n", noderef->FirstChildElement("texture")->Attribute("id"));

						// repeat for other leaf details
					}
					else
						printf("  - Node id: '%s': NOT FOUND IN THE NODES SECTION\n", child->Attribute("id"));

				}
				if (strcmp(child->Value(),"Leaf")==0)
				{
					// access leaf data by searching for its id in the leaves section
					TiXmlElement *leaf=findChildByAttribute(texturesElement,"id",child->Attribute("id"));

					if (leaf)
					{
						// it is a leaf and it is present in the leaves section
						printf("  - Leaf id: '%s' ; type: '%s'\n", child->Attribute("id"), leaf->Attribute("type"));

						// repeat for other leaf details
					}
					else
						printf("  - Leaf id: '%s' - NOT FOUND IN THE LEAVES SECTION\n",child->Attribute("id"));
				}

				child=child->NextSiblingElement();
			}
			node=node->NextSiblingElement();
		}
	}

}

XMLScene::~XMLScene()
{
	delete(doc);
}

//-------------------------------------------------------

TiXmlElement *XMLScene::findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val)
// Searches within descendants of a parent for a node that has an attribute _attr_ (e.g. an id) with the value _val_
// A more elaborate version of this would rely on XPath expressions
{
	TiXmlElement *child=parent->FirstChildElement();
	int found=0;

	while (child && !found)
		if (child->Attribute(attr) && strcmp(child->Attribute(attr),val)==0)
			found=1;
		else
			child=child->NextSiblingElement();

	return child;
}

vector<string> XMLScene::getStringValues(TiXmlElement* element, vector<string> attributes) {

	vector<string> values;
	string valString;

	for(size_t i = 0; i < attributes.size(); i++) {

		valString = globalsElement->Attribute(attributes.at(i).c_str());

		values.push_back(valString);

		if(!valString.empty()) {
			printf("  %s value: %s\n", attributes.at(i).c_str(), valString.c_str());
		}
		else
			printf("Error parsing %s\n", attributes.at(i).c_str());
	}

	return values;
}

vector<float> XMLScene::getFloatValues(TiXmlElement * element, char * attributeName) {
    vector<float> values;
    float value;
    
    char* valString = NULL;
    valString = (char *) element->Attribute(attributeName);
    
    // TODO throw exception? 
    if (valString == NULL ) return values;
    
    stringstream ss (valString);
    while (ss >> value) {
        values.push_back(value);
    }
    
    return values;
}

