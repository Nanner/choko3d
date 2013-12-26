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


string boards[] =     {"boardScene.yaf",  "boardScene2.yaf", "boardScene3.yaf",
                       "boardScene4.yaf", "boardScene5.yaf", "boardScene6.yaf"};
string boardNames[] = {"Ryfjallet, Sweden",
                       "Niagara Falls, Canada",
                       "Larnaca Castle, Cyprus",
                       "Piazza del popolo, Italy",
                       "Skansen, Stockholm",
                       "Royal Castle, Stockholm"};

/* // for faster loading
string boards[] = {"boardScene.yaf" };
string boardNames[] = {"Ryfjallet"};
 */

int main(int argc, char* argv[]) {
    
	CGFapplication app = CGFapplication();
    vector<string> scenes(begin(boards), end(boards));
    vector<string> scenesNames(begin(boardNames), end(boardNames));


	try {
		app.init(&argc, argv);
		SceneGraph* sceneGraph = new SceneGraph(scenes);
        RendererInterface * rendererInterface = new RendererInterface(scenesNames);
		app.setScene(new DemoScene(sceneGraph, rendererInterface));
		app.setInterface(rendererInterface);
		
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