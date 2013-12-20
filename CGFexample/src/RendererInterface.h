#ifndef RENDERERINTERFACE_H
#define RENDERERINTERFACE_H

#include <map>
#include <sstream>
#include <string>

#include "CGFinterface.h"
#include "DemoScene.h"

#define BUFSIZE 256

class RendererInterface :public CGFinterface {
private:
    SceneGraph * sceneGraph;
    GLUI_StaticText * winnerText;
    GLUI_Panel* gameOverPanel;
public:
	GLuint selectBuf[BUFSIZE];

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

	int gameRestartButtonID;

	RendererInterface();

	void initGUI();
	void processGUI(GLUI_Control *ctrl);
	virtual void processMouse(int button, int state, int x, int y);	
	void performPicking(int x, int y);
	void processHits(GLint hits, GLuint buffer[]); 
    void updateGameOver();
};

#endif