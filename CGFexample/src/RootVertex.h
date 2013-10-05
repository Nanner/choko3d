#ifndef _ROOTVERTEX_H
#define _ROOTVERTEX_H

#include "SceneVertex.h"
#include <map>

class RootVertex: public SceneVertex {
public:
	CGFappearance* defaultAppearance;
	YAFGlobal globals;
    
	map<string, CameraView*> cameras;
	map<string, SceneLight*> lights;
	map<string, int> lightOnControls;
    
	RootVertex(float* matrix, string id, YAFGlobal globals, map<string, YAFCamera> cameras, map<string, YAFLight> lights);
    
	void loadDefaultAppearance();
	void setGlobals();
	void setInitialCamera();
};

#endif
