#include "RendererInterface.h"


RendererInterface::RendererInterface() {}

void RendererInterface::initGUI() {
	RootVertex* rootVertex = ((DemoScene*) scene)->getSceneGraph()->getRootVertex();

	int lastID = 0;
	GLUI_Panel* lightsPanel = addPanel( (char*)"Lights", 1);
	map<string, SceneLight*>::iterator lightIterator = rootVertex->lights.begin();
	for(int i = 0; lightIterator != rootVertex->lights.end(); lightIterator++, i++) {
		if(i%4 == 0)
			addColumnToPanel(lightsPanel);

		string lightStr = "Light " + lightIterator->first;
		addCheckboxToPanel(lightsPanel, (char*) lightStr.c_str(), &(rootVertex->lightOnControls.find(lightIterator->first)->second), lastID);
		lightMap.insert(pair<int, string>(lastID, lightIterator->first));
		lastID++;
	}

	lightsPanel->align();

	addColumn();

	GLUI_Panel* camerasPanel = addPanel((char*)"Cameras");
	GLUI_Listbox* cameraList = addListboxToPanel(camerasPanel, (char*)"Camera: ", &((DemoScene*) scene)->activeCameraNum, lastID);
	lastID++;
	map<string, CameraView*>::iterator cameraIterator = rootVertex->cameras.begin();
	cameraList->add_item(0, (char*)"Default");
	for(int i = 1; cameraIterator != rootVertex->cameras.end(); cameraIterator++, i++) {
		cameraList->add_item(i, (char*)cameraIterator->first.c_str());
		if(cameraIterator->first == YAFCamera::initialCameraID)
			cameraList->set_int_val(i);
	}

	camResetButton = addButtonToPanel(camerasPanel, (char*)"Reset camera view", lastID);
	camResetButtonID = lastID;
	lastID++;
	camerasPanel->align();

	addColumn();

	GLUI_Panel* drawmodesPanel = addPanel((char*)"Draw modes");
	GLUI_Listbox* drawmodeList = addListboxToPanel(drawmodesPanel, (char*)"Draw mode: ", &((DemoScene*)scene)->activeDrawMode, lastID);
	lastID++;
	drawmodeList->add_item(0, (char*)"Fill");
	drawmodeList->add_item(1, (char*)"Line");
	drawmodeList->add_item(2, (char*)"Point");
	if(rootVertex->globals.drawmode == "fill")
		drawmodeList->set_int_val(0);
	else if(rootVertex->globals.drawmode == "line")
		drawmodeList->set_int_val(1);
	else if(rootVertex->globals.drawmode == "point")
		drawmodeList->set_int_val(2);
}

void RendererInterface::processGUI(GLUI_Control *ctrl) {

	if( ((DemoScene*) scene)->activeCameraNum == 0) {
		camResetButton->disable();
	}
	else
		camResetButton->enable();

	RootVertex* rootVertex = ((DemoScene*) scene)->getSceneGraph()->getRootVertex();

	map<int, string>::iterator lightToToggle = lightMap.find(ctrl->user_id);
	if(lightToToggle != lightMap.end()) {
		if (!rootVertex->lightOnControls.find(lightToToggle->second)->second){
			rootVertex->lights.find(lightToToggle->second)->second->disable();
		} else {
			rootVertex->lights.find(lightToToggle->second)->second->enable();
		}
		return;
	}

	if(ctrl->user_id == camResetButtonID && camResetButton->enabled) {
		((DemoScene*) scene)->resetCurrentCamera();
	}
}
