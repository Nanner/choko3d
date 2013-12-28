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
    GLUI * gameOverWindow;
    bool gameOverWindowVisible;

	GLUI_StaticText * filmOverText;
	GLUI_Panel* filmOverPanel;
	GLUI * filmOverWindow;
	bool filmOverWindowVisible;

    vector<string> sceneNames;
public:
	bool popupWindowOn;
    int mainWindow;
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

	int undoButtonID;
	GLUI_Button* undoButton;
    
    GLUI * noMovesWindow;
    GLUI_Panel * noMovesPanel;
    int turnSkipButtonID;
    bool noMovesWindowVisible;

	int gameOverGameRestartButtonID;
	int gameOverFilmButtonID;

	int filmOverGameRestartButtonID;
	int filmOverFilmButtonID;
    
	GLUI_Checkbox* cameraRotationCheckbox;
    int cameraRotationID;
	bool autoCameraOn;

	GLUI_Listbox* sceneList;
    int sceneSwitchID;

	RendererInterface(vector<string> sceneNames);

	void initGUI();
	void processGUI(GLUI_Control *ctrl);
	virtual void processMouse(int button, int state, int x, int y);	
	void performPicking(int x, int y);
	void processHits(GLint hits, GLuint buffer[]); 
    void updateGameOver();
    void updateNoMoves();
	void updateFilmOver();
    void deselectCurrentPiece();
};

#endif