#include "YAFReader.h"

//TODO Check for missing blocks of info on the .yaf and terminate if such is found!

string Camera::initialCameraID = "";

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
			for (unsigned int i = 0; i < backgroundAttributes.size(); i++) {
				printf("%s: %s \n", bgAttributeNames.at(i).c_str(), backgroundAttributes.at(i).c_str());
			}

			try {
				yafGlobals = Global(rgba, backgroundAttributes);
			}
			catch (InvalidAttributeValueException &ive) {
				cout << ive.what() << endl;
				exit(1);
			}	
		}

		// -------------- CAMERAS -----------------------------------------

		if (camerasElement == NULL) {
			printf("Cameras block not found!\n");
			exit(1);
		}
		else
		{
			printf("Processing cameras:\n");

			Camera::initialCameraID = getValue<string>(camerasElement, (char*) "initial");

			TiXmlElement* currentElement = camerasElement->FirstChildElement();
			if(currentElement == NULL) {
				printf("No cameras found!\n");
				exit(1);
			}

			while (currentElement) {

				if ( strcmp(currentElement->Value(), "perspective") == 0 )
				{
					// TODO store value
					string id = getValue<string>(currentElement, (char*) "id");

					vector<string> nfa; // near, far, angle
					nfa.push_back("near"); nfa.push_back("far"); nfa.push_back("angle");
					vector<float> nfaValues = getValues<float>(currentElement, nfa);

					// TODO store values, remove print
					for (unsigned int i = 0; i < nfaValues.size(); i++) {
						printf("%s: %f\n", nfa.at(i).c_str(), nfaValues.at(i));
					}

					// TODO store values
					vector<float> position = getValues<float>(currentElement, (char*)"pos");

					// TODO store values
					vector<float> target = getValues<float>(currentElement, (char*)"target");
					for (unsigned int i = 0; i < target.size(); i++) {
						printf("target: %f\n", target.at(i));
					}

					Camera perspectiveCamera(id, nfaValues, position, target);
					bool notRepeated = yafCameras.insert(pair<string, Camera>(perspectiveCamera.id, perspectiveCamera)).second;
					if(!notRepeated) {
						printf("Tried to insert a camera with an already existing camera id. Terminating!\n");
						exit(1);
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

					Camera orthoCamera(id, orthoValues);
					bool notRepeated = yafCameras.insert(pair<string, Camera>(orthoCamera.id, orthoCamera)).second;
					if(!notRepeated) {
						printf("Tried to insert a camera with an already existing camera id. Terminating!\n");
						exit(1);
					}
				}

				currentElement = currentElement->NextSiblingElement();

			}
		}

		// -------------- LIGHTS -----------------------------------------

		if (lightingElement == NULL) {
			printf("Lightning block not found!\n");
			exit(1);
		}
		else
		{
			printf("Processing lights:\n");

			vector<string> attributeNames;
			attributeNames.push_back("doublesided");
			attributeNames.push_back("local"); attributeNames.push_back("enabled");

			vector<bool> attributes = getValues<bool>(lightingElement, attributeNames);

			// TODO store values, remove this print
			for (unsigned int i = 0; i < attributes.size(); i++) {
				cout << boolalpha << attributes.at(i) << endl;
			}

			vector<float> ambient = getValues<float>(lightingElement, (char*) "ambient");

			TiXmlElement* currentElement = lightingElement->FirstChildElement();
			if(currentElement == NULL) {
				printf("No lights found!\n");
				exit(1);
			}

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

					vector<float> location = getValues<float>(currentElement, (char*)"location");

					vector<float> ambient = getValues<float>(currentElement, (char*)"ambient");

					vector<float> diffuse = getValues<float>(currentElement, (char*)"diffuse");

					vector<float> specular = getValues<float>(currentElement, (char*)"specular");

					float angle = getValue<float>(currentElement, (char*)"angle");

					float exponent = getValue<float>(currentElement, (char*)"exponent");

					vector<float> direction = getValues<float>(currentElement, (char*)"direction");
				}

				currentElement = currentElement->NextSiblingElement();

			}
		}

		// -------------- TEXTURES -----------------------------------------

		if (texturesElement == NULL) {
			printf("Texture block not found!\n");
			exit(1);
		}
		else
		{
			printf("Processing textures:\n");

			TiXmlElement* textureElement = texturesElement->FirstChildElement("texture");

			while ( textureElement ) {
				// TODO store values
				string id = getValue<string>(textureElement, (char*)"id");
				string file = getValue<string>(textureElement, (char*)"file");

				textureElement = textureElement->NextSiblingElement();
			}

		}


		// -------------- APPEARANCES -----------------------------------------

		if (appearancesElement == NULL) {
			printf("Appearances block not found!\n");
			exit(1);
		}
		else
		{
			printf("Processing appearances:\n");

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
				}
				catch (EmptyAttributeException &eae) {}

				appearanceElement = appearanceElement->NextSiblingElement();
			}

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
		cout << endl << iave.what() << endl << endl << "Terminating parser." << endl;
		exit(1);
	}
	catch (EmptyAttributeException &eae) {
		cout << endl << eae.what() << endl << endl << "Terminating parser." << endl;
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
