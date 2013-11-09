#include <map>
#include <sstream>
#include <string>

#include "CGFinterface.h"
#include "DemoScene.h"

class RendererInterface :public CGFinterface {
public:
	map<int, string> lightMap;

	int camResetButtonID;
	GLUI_Button* camResetButton;

	bool animationsExist;
	map<int, string> animationMap;
	int selectedAnimation;
	int animationListboxID;
	int animationResetButtonID;
	int allAnimationResetButtonID;
	GLUI_Checkbox* animationLoopCheckbox;
	int animationLoopCheckboxID;
	GLUI_Button* animationPauseButton;
	int animationPauseButtonID;
	bool animationPaused;

	int speedSpinnerID;
	int heightSpinnerID;
	int inclineSpinnerID;

	int displayListCheckboxID;

	RendererInterface();

	void initGUI();
	void processGUI(GLUI_Control *ctrl);
};

