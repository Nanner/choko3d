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

	try {
		app.init(&argc, argv);
		YAFReader* yafFile = new YAFReader( (char*)"donutScene.yaf");
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