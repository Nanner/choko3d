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

