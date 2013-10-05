#include <map>
#include <sstream>
#include <string>

#include "CGFinterface.h"
#include "DemoScene.h"

class RendererInterface :public CGFinterface {
public:
	map<int, string> lightMap;
	RendererInterface();

	void initGUI();
	void processGUI(GLUI_Control *ctrl);
};

