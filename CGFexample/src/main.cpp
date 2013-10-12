/* 
 * G0_Base: projeto inicial de CGra
 * 
 */

#include <iostream>
#include <exception>

#include "CGFapplication.h"
#include "DemoScene.h"
#include "YAFReader.h"
#include "SceneGraph.h"
#include "ScenePrimitive.h"
#include "SceneVertex.h"
#include "Transformation.h"
#include "RendererInterface.h"
#include <math.h>

using std::cout;
using std::exception;


int main(int argc, char* argv[]) {
    
	CGFapplication app = CGFapplication();
    char yafFileName[512];
    
    if (argc < 2) {
        printf("Usage:./YafEngine filename.yaf\n");
        printf("No YAF file has been specified in the arguments.\n");
        printf("Please enter the filename of the scene:\n> ");
        scanf("%s", yafFileName);
    } else {
        strcpy(yafFileName, argv[1]);
    }

	try {
		app.init(&argc, argv);
		YAFReader* yafFile = new YAFReader( yafFileName );
		SceneGraph* sceneGraph = new SceneGraph(yafFile);
		delete(yafFile);
		app.setScene(new DemoScene(sceneGraph));
		app.setInterface(new RendererInterface());
		
		app.run();
	}
	catch(GLexception& ex) {
		cout << "Error: " << ex.what();
		return -1;
	}
	catch(exception& ex) {
		cout << "Unexpected error: " << ex.what();
		return -1;
	}

	return 0;
}