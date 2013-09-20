#include "YAFReader.h"

YAFReader::YAFReader(char *filename)
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

    // -------------- GLOBALS -----------------------------------------
    
	if (globalsElement == NULL)
		printf("Globals block not found!\n");
	else
	{
		printf("Processing globals:\n");

		vector<float> rgba = getValues<float>(globalsElement, (char*) "background");

        // TODO store rgba values, remove print
        printf("  background values (RGBA): %f %f %f %f\n", rgba.at(0), rgba
               .at(1), rgba.at(2), rgba.at(3));
        
        vector<string> bgAttributeNames;
        bgAttributeNames.push_back("drawmode"); bgAttributeNames.push_back("shading");
        bgAttributeNames.push_back("cullface"); bgAttributeNames.push_back("cullorder");

		vector<string> backgroundAttributes = getValues<string>(globalsElement, bgAttributeNames);

        // TODO store background attributes, remove print
        for (int i = 0; i < backgroundAttributes.size(); i++) {
            printf("%s: %s \n", bgAttributeNames.at(i).c_str(), backgroundAttributes.at(i).c_str());
        }

		try {
			GlobalAttributes globalsBlock(rgba, backgroundAttributes);
		}
		catch (InvalidAttributeValueException &ive) {
			cout << ive.what() << endl;
			exit(1);
		}	
	}

    // -------------- CAMERAS -----------------------------------------

    if (camerasElement == NULL)
		printf("Cameras block not found!\n");
	else
	{
        printf("Processing cameras:\n");
        
        string initial = getValue<string>(camerasElement, (char*) "initial");
        
        TiXmlElement* currentElement = camerasElement->FirstChildElement();
        
        while (currentElement) {
            
            if ( strcmp(currentElement->Value(), "perspective") == 0 )
            {
                // TODO store value
                string id = getValue<string>(currentElement, (char*) "id");
                
                vector<string> nfa; // near, far, angle
                nfa.push_back("near"); nfa.push_back("far"); nfa.push_back("angle");
                vector<float> nfaValues = getValues<float>(currentElement, nfa);
                
                // TODO store values, remove print
                for (int i = 0; i < nfaValues.size(); i++) {
                    printf("%s: %f\n", nfa.at(i).c_str(), nfaValues.at(i));
                }
                
                // TODO store values
                vector<float> position = getValues<float>(currentElement, (char*)"pos");
                
                // TODO store values
                vector<float> target = getValues<float>(currentElement, (char*)"target");
                for (int i = 0; i < target.size(); i++) {
                    printf("target: %f\n", target.at(i));
                }
                
            }
            
            if (  strcmp(currentElement->Value(), "ortho") == 0  )
            {
                // TODO store value
                string id = getValue<string>(currentElement, (char*)"id");
                
                vector<string> orthoAttributes;
                orthoAttributes.push_back("near"); orthoAttributes.push_back("far");
                orthoAttributes.push_back("left"); orthoAttributes.push_back("right");
                orthoAttributes.push_back("top"); orthoAttributes.push_back("bottom");
                
                // TODO store values
                vector<float> orthoValues = getValues<float>(currentElement, orthoAttributes);
            }
            
            currentElement = currentElement->NextSiblingElement();
            
        }
    }
    
    // -------------- LIGHTS -----------------------------------------
    
    if (lightingElement == NULL)
		printf("Lightning block not found!\n");
	else
	{
        printf("Processing lights:\n");
        
        vector<string> attributeNames;
        attributeNames.push_back("doublesided");
        attributeNames.push_back("local"); attributeNames.push_back("enabled");
        
        vector<bool> attributes = getValues<bool>(lightingElement, attributeNames);
        
        // TODO store values, remove this print
        for (int i = 0; i < attributes.size(); i++) {
            cout << boolalpha << attributes.at(i) << endl;
        }
        
        vector<float> ambient = getValues<float>(lightingElement, (char*) "ambient");
        
        TiXmlElement* currentElement = lightingElement->FirstChildElement();
        
        while (currentElement) {
            
            if ( strcmp(currentElement->Value(), "omni") == 0 )
            {
                // TODO store value
                string id = getValue<string>(currentElement, (char*) "id");
                
                bool enabled = getValue<bool>(currentElement, (char*)"enabled");
                
                vector<float> location = getValues<float>(currentElement, (char*)"location");
                
                // TODO 2 named ambient's, shouldnt this cause an error?
                vector<float> ambient = getValues<float>(currentElement, (char*)"ambient");

                vector<float> diffuse = getValues<float>(currentElement, (char*)"diffuse");

                vector<float> specular = getValues<float>(currentElement, (char*)"specular");
            }
            
            if (  strcmp(currentElement->Value(), "spot") == 0  )
            {
                // TODO store value
                string id = getValue<string>(currentElement, (char*)"id");
                
                bool enabled = getValue<bool>(currentElement, (char*)"enabled");
                
                vector<string> spotAttributes;
                spotAttributes.push_back("angle"); spotAttributes.push_back("exponent");
                
                // TODO store values
                vector<float> spotValues = getValues<float>(currentElement, spotAttributes);
                
                vector<float> direction = getValues<float>(currentElement, (char*)"direction");
            }
            
            currentElement = currentElement->NextSiblingElement();
            
        }
    }
    
    // -------------- TEXTURES -----------------------------------------
    
    if (texturesElement == NULL)
		printf("Texture block not found!\n");
	else
	{
        printf("Processing textures:\n");
        
        TiXmlElement* textureElement = texturesElement->FirstChildElement("texture");
        
        while ( textureElement ) {
            // TODO store values
            string id = getValue<string>(texturesElement, (char*)"id");
            string file = getValue<string>(texturesElement, (char*)"file");
            
            textureElement = textureElement->NextSiblingElement();
        }
        
    }
    
    
    // -------------- APPEARANCES -----------------------------------------
    
    if (appearancesElement == NULL)
		printf("Appearances block not found!\n");
	else
	{
        printf("Processing appearances:\n");
        
        TiXmlElement* appearanceElement = appearancesElement->FirstChildElement("appearance");
        
        while ( appearanceElement ) {
            // TODO store values
            string id = getValue<string>(appearanceElement, (char*)"id");
            
            vector<float> emissive = getValues<float>(appearanceElement, (char*)"emissive");
            
            vector<float> ambient = getValues<float>(appearanceElement, (char*)"ambient");
            
            vector<float> diffuse = getValues<float>(appearanceElement, (char*)"diffuse");
            
            vector<float> specular = getValues<float>(appearanceElement, (char*)"specular");

            float shininess = getValue<float>(appearanceElement, (char*)"shininess");
            
            string textureref = getValue<string>(appearanceElement, (char*)"textureref");
            
            float texlength_s = getValue<float>(appearanceElement, (char*) "texlength_s");
            
            float texlength_t = getValue<float>(appearanceElement, (char*) "texlength_t");

            appearanceElement = appearanceElement->NextSiblingElement();
        }
        
    }


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

YAFReader::~YAFReader()
{
	delete(doc);
}

//-------------------------------------------------------

TiXmlElement *YAFReader::findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val)
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
