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

using std::cout;
using std::exception;


int main(int argc, char* argv[]) {

	CGFapplication app = CGFapplication();

	try {
		app.init(&argc, argv);

		/*SceneGraph* sceneGraph = new SceneGraph();

		Rectangle* rect = new Rectangle(0.5, 0.5, -0.5, -0.5);

		Rotation* rot1 = new Rotation("x", 270);
		vector<float> transVal1;
		transVal1.push_back(0.0); transVal1.push_back(0.5); transVal1.push_back(0.0);
		Translation* trans1 = new Translation(transVal1);
		vector<Transformation *> transf1; transf1.push_back(rot1); transf1.push_back(trans1);
		float* matrix1 = Transformation::calculateMatrix(transf1);
		SceneComposite* top = new SceneComposite(matrix1);

		Rotation* rot2 = new Rotation("x", 0);
		vector<float> transVal2;
		transVal2.push_back(0.0); transVal2.push_back(0.0); transVal2.push_back(0.5);
		Translation* trans2 = new Translation(transVal2);
		vector<Transformation *> transf2; transf2.push_back(rot2); transf2.push_back(trans2);
		float* matrix2 = Transformation::calculateMatrix(transf2);
		SceneComposite* front = new SceneComposite(matrix2);

		Rotation* rot3 = new Rotation("y", 180);
		vector<float> transVal3;
		transVal3.push_back(0.0); transVal3.push_back(0.0); transVal3.push_back(-0.5);
		Translation* trans3 = new Translation(transVal3);
		vector<Transformation *> transf3; transf3.push_back(rot3); transf3.push_back(trans3);
		float* matrix3 = Transformation::calculateMatrix(transf3);
		SceneComposite* back = new SceneComposite(matrix3);

		Rotation* rot4 = new Rotation("x", 90);
		vector<float> transVal4;
		transVal4.push_back(0.0); transVal4.push_back(-0.5); transVal4.push_back(0.0);
		Translation* trans4 = new Translation(transVal4);
		vector<Transformation *> transf4; transf4.push_back(rot4); transf4.push_back(trans4);
		float* matrix4 = Transformation::calculateMatrix(transf4);
		SceneComposite* bottom = new SceneComposite(matrix4);

		Rotation* rot5 = new Rotation("y", 270);
		vector<float> transVal5;
		transVal5.push_back(-0.5); transVal5.push_back(0.0); transVal5.push_back(0.0);
		Translation* trans5 = new Translation(transVal5);
		vector<Transformation *> transf5; transf5.push_back(rot5); transf5.push_back(trans5);
		float* matrix5 = Transformation::calculateMatrix(transf5);
		SceneComposite* left = new SceneComposite(matrix5);

		Rotation* rot6 = new Rotation("y", 90);
		vector<float> transVal6;
		transVal6.push_back(0.5); transVal6.push_back(0.0); transVal6.push_back(0.0);
		Translation* trans6 = new Translation(transVal6);
		vector<Transformation *> transf6; transf6.push_back(rot6); transf6.push_back(trans6);
		float* matrix6 = Transformation::calculateMatrix(transf6);
		SceneComposite* right = new SceneComposite(matrix6);

		Rotation* rot7 = new Rotation("x", 45);
		vector<float> transVal7;
		transVal7.push_back(0.0); transVal7.push_back(2.0); transVal7.push_back(2.0);
		Translation* trans7 = new Translation(transVal7);
		vector<float> scalingVal7;
		scalingVal7.push_back(5.0); scalingVal7.push_back(5.0); scalingVal7.push_back(5.0);
		Scaling* scale7 = new Scaling(scalingVal7);
		vector<Transformation *> transf7; transf7.push_back(rot7); transf7.push_back(trans7); transf7.push_back(scale7);
		float* matrix7 = Transformation::calculateMatrix(transf7);

		vector<float> triPoint1; triPoint1.push_back(0.0); triPoint1.push_back(0.0); triPoint1.push_back(0.0);
		vector<float> triPoint2; triPoint2.push_back(1.0); triPoint2.push_back(0.0); triPoint2.push_back(1.0);
		vector<float> triPoint3; triPoint3.push_back(0.5); triPoint3.push_back(sqrt(3)/2); triPoint3.push_back(0.5);*/

		/*Triangle* tri = new Triangle(triPoint1, triPoint2, triPoint3);
		sceneGraph->addVertex(tri);

		Cylinder* cyl = new Cylinder(3, 3, 3, 150, 3);
		sceneGraph->addVertex(cyl);*/

		/*Sphere* sphere = new Sphere(5, 100, 30);
		sceneGraph->addVertex(sphere);*/

		/*Torus* torus = new Torus(1, 2, 100, 40);
		sceneGraph->addVertex(torus);
		
		SceneComposite* pseudoCube = new SceneComposite(matrix7);
		sceneGraph->addVertex(pseudoCube);
		sceneGraph->addVertex(top);
		sceneGraph->addVertex(front);
		sceneGraph->addVertex(back);
		sceneGraph->addVertex(bottom);
		sceneGraph->addVertex(left);
		sceneGraph->addVertex(right);
		sceneGraph->addEdge(pseudoCube, top);
		sceneGraph->addEdge(pseudoCube, front);
		sceneGraph->addEdge(pseudoCube, back);
		sceneGraph->addEdge(pseudoCube, bottom);
		sceneGraph->addEdge(pseudoCube, left);
		sceneGraph->addEdge(pseudoCube, right);
		sceneGraph->addVertex(rect);
		sceneGraph->addEdge(top, rect);
		sceneGraph->addEdge(front, rect);
		sceneGraph->addEdge(back, rect);
		sceneGraph->addEdge(bottom, rect);
		sceneGraph->addEdge(left, rect);
		sceneGraph->addEdge(right, rect);*/

		YAFReader* yafFile = new YAFReader( (char*)"scene.xml");
		SceneGraph* sceneGraph = new SceneGraph(yafFile);
		delete(yafFile);
		app.setScene(new DemoScene(sceneGraph));
		app.setInterface(new CGFinterface());
		
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