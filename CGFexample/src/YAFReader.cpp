#include "YAFReader.h"

//TODO Check for missing blocks of info on the .yaf and terminate if such is found!

YAFReader::YAFReader(char *filename) {
	try {
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

		if (globalsElement == NULL) {
			printf("Globals block not found!\n");
			exit(1);
		}
		else
		{
			printf("Processing globals... ");

			vector<float> rgba = getValues<float>(globalsElement, (char*) "background");

			vector<string> bgAttributeNames;
			bgAttributeNames.push_back("drawmode"); bgAttributeNames.push_back("shading");
			bgAttributeNames.push_back("cullface"); bgAttributeNames.push_back("cullorder");

			vector<string> backgroundAttributes = getValues<string>(globalsElement, bgAttributeNames);

            globals = YAFGlobal(rgba, backgroundAttributes);
            
            printf("Globals OK!\n");
		}

		// -------------- CAMERAS -----------------------------------------

		if (camerasElement == NULL) {
			printf("Cameras block not found!\n");
			exit(1);
		}
		else
		{
			printf("Processing cameras... ");

			YAFCamera::initialCameraID = getValue<string>(camerasElement, (char*) "initial");

			TiXmlElement* currentCamera = camerasElement->FirstChildElement();
			if(currentCamera == NULL) {
				printf("No cameras found!\n");
				exit(1);
			}

			while (currentCamera) {

				if ( strcmp(currentCamera->Value(), "perspective") == 0 )
				{
					string id = getValue<string>(currentCamera, (char*) "id");

					vector<string> nfa; // near, far, angle
					nfa.push_back("near"); nfa.push_back("far"); nfa.push_back("angle");
					vector<float> nfaValues = getValues<float>(currentCamera, nfa);

					vector<float> position = getValues<float>(currentCamera, (char*)"pos");

					vector<float> target = getValues<float>(currentCamera, (char*)"target");

					YAFCamera perspective(id, nfaValues, position, target);
					bool notRepeated = cameras.insert(pair<string, YAFCamera>(id, perspective)).second;
					if(!notRepeated) {
						printf("Tried to insert a camera with an already existing camera id. Terminating!\n");
						exit(1);
					}

				}

				if (  strcmp(currentCamera->Value(), "ortho") == 0  )
				{
					string id = getValue<string>(currentCamera, (char*)"id");

					vector<string> orthoAttributes;
					orthoAttributes.push_back("near"); orthoAttributes.push_back("far");
					orthoAttributes.push_back("left"); orthoAttributes.push_back("right");
					orthoAttributes.push_back("top"); orthoAttributes.push_back("bottom");

					vector<float> orthoValues = getValues<float>(currentCamera, orthoAttributes);

					YAFCamera ortho(id, orthoValues);
					bool notRepeated = cameras.insert(pair<string, YAFCamera>(id, ortho)).second;
					if(!notRepeated) {
						printf("Tried to insert a camera with an already existing camera id. Terminating!\n");
						exit(1);
					}
				}

				currentCamera = currentCamera->NextSiblingElement();

			}
            
            printf("Cameras OK!\n");
		}

		// -------------- LIGHTS -----------------------------------------

		if (lightingElement == NULL) {
			printf("Lightning block not found!\n");
			exit(1);
		}
		else
		{
			printf("Processing lights... ");

			vector<string> attributeNames;
			attributeNames.push_back("doublesided");
			attributeNames.push_back("local"); attributeNames.push_back("enabled");

			vector<bool> attributes = getValues<bool>(lightingElement, attributeNames);

			vector<float> ambient = getValues<float>(lightingElement, (char*) "ambient");
            
            globalLighting = YAFGlobalLighting(attributes, ambient);

			TiXmlElement* currentLight = lightingElement->FirstChildElement();
			if(currentLight == NULL) {
				printf("No lights found!\n");
				exit(1);
			}

			while (currentLight) {

				if ( strcmp(currentLight->Value(), "omni") == 0 )
				{
					string id = getValue<string>(currentLight, (char*) "id");

					bool enabled = getValue<bool>(currentLight, (char*)"enabled");

					vector<float> location = getValues<float>(currentLight, (char*)"location");

					// TODO 2 named ambient's, shouldnt this cause an error?
					vector<float> ambient = getValues<float>(currentLight, (char*)"ambient");

					vector<float> diffuse = getValues<float>(currentLight, (char*)"diffuse");

					vector<float> specular = getValues<float>(currentLight, (char*)"specular");
                    
                    YAFLight omni(id, enabled, location, ambient, diffuse, specular);
					bool notRepeated = lights.insert(pair<string, YAFLight>(id, omni)).second;
					if(!notRepeated) {
						printf("Tried to insert a omni light with an already existing light id. Terminating!\n");
						exit(1);
					}
				}

				if (  strcmp(currentLight->Value(), "spot") == 0  )
				{
					string id = getValue<string>(currentLight, (char*)"id");

					bool enabled = getValue<bool>(currentLight, (char*)"enabled");

					vector<float> location = getValues<float>(currentLight, (char*)"location");

					vector<float> ambient = getValues<float>(currentLight, (char*)"ambient");

					vector<float> diffuse = getValues<float>(currentLight, (char*)"diffuse");

					vector<float> specular = getValues<float>(currentLight, (char*)"specular");

					float angle = getValue<float>(currentLight, (char*)"angle");

					float exponent = getValue<float>(currentLight, (char*)"exponent");

					vector<float> direction = getValues<float>(currentLight, (char*)"direction");
                    
                    YAFLight spot(id, enabled, location, ambient, diffuse, specular, angle, exponent, direction);
                    
					bool notRepeated = lights.insert(pair<string, YAFLight>(id, spot)).second;
					if(!notRepeated) {
						printf("Tried to insert a spot light with an already existing light id. Terminating!\n");
						exit(1);
					}
				}

				currentLight = currentLight->NextSiblingElement();

			}
            
            printf("Lights OK!\n");
		}
        
		// -------------- TEXTURES -----------------------------------------

		if (texturesElement == NULL) {
			printf("Texture block not found!\n");
			exit(1);
		}
		else
		{
			printf("Processing textures... ");

			TiXmlElement* textureElement = texturesElement->FirstChildElement("texture");

			while ( textureElement ) {
				// TODO store values
				string id = getValue<string>(textureElement, (char*)"id");
				string file = getValue<string>(textureElement, (char*)"file");

				YAFTexture texture(id, file);

				bool notRepeated = textures.insert(pair<string, YAFTexture>(id, texture)).second;
				if(!notRepeated) {
					printf("Tried to insert a texture with an already existing texture id. Terminating!\n");
					exit(1);
				}

				textureElement = textureElement->NextSiblingElement();
			}

			printf("Textures OK!\n");
		}


		// -------------- APPEARANCES -----------------------------------------

		if (appearancesElement == NULL) {
			printf("Appearances block not found!\n");
			exit(1);
		}
		else
		{
			printf("Processing appearances... ");

			TiXmlElement* appearanceElement = appearancesElement->FirstChildElement("appearance");
			if(appearanceElement == NULL) {
				printf("No appearances found!\n");
				exit(1);
			}

			while ( appearanceElement ) {
				// TODO store values
				string id = getValue<string>(appearanceElement, (char*)"id");

				vector<float> emissive = getValues<float>(appearanceElement, (char*)"emissive");

				vector<float> ambient = getValues<float>(appearanceElement, (char*)"ambient");

				vector<float> diffuse = getValues<float>(appearanceElement, (char*)"diffuse");

				vector<float> specular = getValues<float>(appearanceElement, (char*)"specular");

				float shininess = getValue<float>(appearanceElement, (char*)"shininess");

				try {
					string textureref = getValue<string>(appearanceElement, (char*)"textureref");

					float texlength_s = getValue<float>(appearanceElement, (char*) "texlength_s");

					float texlength_t = getValue<float>(appearanceElement, (char*) "texlength_t");

					YAFAppearance apperance(id, emissive, ambient, diffuse, specular, shininess, textureref, texlength_s, texlength_t, textures);

					bool notRepeated = appearances.insert(pair<string, YAFAppearance>(id, apperance)).second;
					if(!notRepeated) {
						printf("Tried to insert an appearance with an already existing appearance id. Terminating!\n");
						exit(1);
					}
				}
				catch (EmptyAttributeException &eae) {
					YAFAppearance apperance(id, emissive, ambient, diffuse, specular, shininess);

					bool notRepeated = appearances.insert(pair<string, YAFAppearance>(id, apperance)).second;
					if(!notRepeated) {
						printf("Tried to insert an appearance with an already existing appearance id. Terminating!\n");
						exit(1);
					}
				}


				appearanceElement = appearanceElement->NextSiblingElement();
			}

			printf("Appearances OK!\n");
		}


		// graph section
		if (graphElement == NULL) {
			printf("Graph block not found!\n");
			exit(1);
		}
		else
		{
			// TODO store rootid
			string rootid = getValue<string>(graphElement, (char*)"rootid");

			TiXmlElement *node = graphElement->FirstChildElement();

			while (node)
			{
				printf("Node id '%s' - Descendants:\n",node->Attribute("id"));

				TiXmlElement * transforms = node->FirstChildElement("transforms");

				if ( transforms == NULL) {
					// TODO better error handling
					printf("obligatory transforms block doesn't exist!");
				}

				TiXmlElement * currentTransform = transforms->FirstChildElement();

				while (currentTransform) {
					if ( strcmp(currentTransform->Value(), "translate") == 0 ) {
						// TODO store translation
						vector<float> to = getValues<float>(currentTransform, (char*)"to");
					}

					if ( strcmp(currentTransform->Value(), "rotate") == 0 ) {
						// TODO store translation
						char axis = getValue<char>(currentTransform, (char*)"axis");
						float angle = getValue<float>(currentTransform, (char*)"angle");
					}

					if ( strcmp(currentTransform->Value(), "scale") == 0 ) {
						// TODO store translation
						vector<float> factor = getValues<float>(currentTransform, (char*)"factor");
					}

					currentTransform = currentTransform->NextSiblingElement();
				}

				TiXmlElement * appearanceref = node->FirstChildElement("appearanceref");

				if (appearanceref) {
					// TODO store appearance
					string appearanceID = getValue<string>(appearanceref, (char*)"id");
				}

				TiXmlElement * children = node->FirstChildElement("children");

				if (children == NULL) {
					// TODO better error handling
					printf("obligatory children block doesn't exist!");
				}

				TiXmlElement * currentChild = children->FirstChildElement();

				int primitiveCounter = 0;
				int nodeRefCounter = 0;

				while (currentChild) {
					if ( strcmp(currentChild->Value(), "noderef") == 0 ) {
						nodeRefCounter++;
						string id = getValue<string>(currentChild, (char*)"id");
					} else {
						// it is not a reference to a node
						// it can only be a primitive now
						primitiveCounter++;
					}

					if ( strcmp(currentChild->Value(), "rectangle") == 0 ) {
						// TODO store primitive
						vector<float> xy1 = getValues<float>(currentChild, (char*)"xy1");
						vector<float> xy2 = getValues<float>(currentChild, (char*)"xy2");
					}

					if ( strcmp(currentChild->Value(), "triangle") == 0 ) {
						// TODO store primitive
						vector<float> xyz1 = getValues<float>(currentChild, (char*)"xyz1");
						vector<float> xyz2 = getValues<float>(currentChild, (char*)"xyz2");
						vector<float> xyz3 = getValues<float>(currentChild, (char*)"xyz3");
					}

					if ( strcmp(currentChild->Value(), "cylinder") == 0 ) {
						// TODO store primitive
						float base = getValue<float>(currentChild, (char*)"base");
						float top = getValue<float>(currentChild, (char*)"top");
						float height = getValue<float>(currentChild, (char*)"height");
						int slices = getValue<int>(currentChild, (char*)"slices");
						int stacks = getValue<int>(currentChild, (char*)"stacks");
					}

					if ( strcmp(currentChild->Value(), "sphere") == 0 ) {
						// TODO store primitive
						float radius = getValue<float>(currentChild, (char*)"radius");
						int slices = getValue<int>(currentChild, (char*)"slices");
						int stacks = getValue<int>(currentChild, (char*)"stacks");
					}

					if ( strcmp(currentChild->Value(), "torus") == 0 ) {
						// TODO store primitive
						float inner = getValue<float>(currentChild, (char*)"inner");
						float outer = getValue<float>(currentChild, (char*)"outer");
						int slices = getValue<int>(currentChild, (char*)"slices");
						int loops = getValue<int>(currentChild, (char*)"loops");
					}

					currentChild = currentChild->NextSiblingElement();
				}

				if (primitiveCounter == 0 && nodeRefCounter == 0) {
					// TODO better error handling
					printf("There must be at least one primitive or one node reference for each child in the children block!");
				}

				node=node->NextSiblingElement();
			}
		}

	}
	catch (InvalidAttributeValueException &iave) {
		cout << endl << iave.error() << endl << endl << "Terminating parser." << endl;
		exit(1);
	}
	catch (EmptyAttributeException &eae) {
		cout << endl << eae.error() << endl << endl << "Terminating parser." << endl;
		exit(1);
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
