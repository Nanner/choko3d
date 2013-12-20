#include "Vehicle.h"


Vehicle::Vehicle() {
	matrix = NULL;

	//Create balloon and its transformation matrix
	balloon = new Sphere(0.5, 10, 20);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 1.7, -2.0);
	glScalef(3.0, 3.0, 14.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, balloonMatrix);
	glPopMatrix();

	//Create cabin patches
	vector<float> cabinPoints;
	cabinPoints.push_back(0.0); cabinPoints.push_back(-0.5); cabinPoints.push_back(-1.5);
	cabinPoints.push_back(0.0); cabinPoints.push_back(0.0); cabinPoints.push_back(-1.5);
	cabinPoints.push_back(0.0); cabinPoints.push_back(0.5); cabinPoints.push_back(-1.5);

	cabinPoints.push_back(1.0); cabinPoints.push_back(-0.5); cabinPoints.push_back(0.0);
	cabinPoints.push_back(1.0); cabinPoints.push_back(0.0); cabinPoints.push_back(0.0);
	cabinPoints.push_back(1.0); cabinPoints.push_back(0.5); cabinPoints.push_back(0.0);

	cabinPoints.push_back(0.0); cabinPoints.push_back(-0.5); cabinPoints.push_back(1.5);
	cabinPoints.push_back(0.0); cabinPoints.push_back(0.0); cabinPoints.push_back(1.5);
	cabinPoints.push_back(0.0); cabinPoints.push_back(0.5); cabinPoints.push_back(1.5);
	
	cabinSide = new Patch(2, 10, 20, "fill", cabinPoints);

	vector<float> cabinBottomPoints;
	cabinBottomPoints.push_back(0.0); cabinBottomPoints.push_back(-0.5); cabinBottomPoints.push_back(1.5);
	cabinBottomPoints.push_back(0.0); cabinBottomPoints.push_back(-0.5); cabinBottomPoints.push_back(1.5);
	cabinBottomPoints.push_back(0.0); cabinBottomPoints.push_back(-0.5); cabinBottomPoints.push_back(1.5);

	cabinBottomPoints.push_back(1.0); cabinBottomPoints.push_back(-0.5); cabinBottomPoints.push_back(0.0);
	cabinBottomPoints.push_back(0.0); cabinBottomPoints.push_back(-1.5); cabinBottomPoints.push_back(0.0);
	cabinBottomPoints.push_back(-1.0); cabinBottomPoints.push_back(-0.5); cabinBottomPoints.push_back(0.0);

	cabinBottomPoints.push_back(0.0); cabinBottomPoints.push_back(-0.5); cabinBottomPoints.push_back(-1.5);
	cabinBottomPoints.push_back(0.0); cabinBottomPoints.push_back(-0.5); cabinBottomPoints.push_back(-1.5);
	cabinBottomPoints.push_back(0.0); cabinBottomPoints.push_back(-0.5); cabinBottomPoints.push_back(-1.5);
	cabinBottom = new Patch(2, 10, 20, "fill", cabinBottomPoints);

	//Create wing patches
	vector<float> wingFrontPoints;
	wingFrontPoints.push_back(0.0); wingFrontPoints.push_back(-1.0); wingFrontPoints.push_back(-1.5);
	wingFrontPoints.push_back(0.0); wingFrontPoints.push_back(0.0); wingFrontPoints.push_back(-1.5);
	wingFrontPoints.push_back(0.0); wingFrontPoints.push_back(0.5); wingFrontPoints.push_back(-1.5);

	wingFrontPoints.push_back(0.0); wingFrontPoints.push_back(-1.0); wingFrontPoints.push_back(0.0);
	wingFrontPoints.push_back(0.0); wingFrontPoints.push_back(0.0); wingFrontPoints.push_back(0.0);
	wingFrontPoints.push_back(0.0); wingFrontPoints.push_back(0.5); wingFrontPoints.push_back(-1.0);

	wingFrontPoints.push_back(0.0); wingFrontPoints.push_back(-1.0); wingFrontPoints.push_back(1.5);
	wingFrontPoints.push_back(0.0); wingFrontPoints.push_back(0.0); wingFrontPoints.push_back(1.5);
	wingFrontPoints.push_back(0.0); wingFrontPoints.push_back(0.5); wingFrontPoints.push_back(-1.0);

	wingFront = new Patch(2, 10, 20, "fill", wingFrontPoints);

	vector<float> wingBackPoints;
	wingBackPoints.push_back(0.0); wingBackPoints.push_back(-1.0); wingBackPoints.push_back(1.5);
	wingBackPoints.push_back(0.0); wingBackPoints.push_back(0.0); wingBackPoints.push_back(1.5);
	wingBackPoints.push_back(0.0); wingBackPoints.push_back(0.5); wingBackPoints.push_back(-1.0);

	wingBackPoints.push_back(0.0); wingBackPoints.push_back(-1.0); wingBackPoints.push_back(0.0);
	wingBackPoints.push_back(0.0); wingBackPoints.push_back(0.0); wingBackPoints.push_back(0.0);
	wingBackPoints.push_back(0.0); wingBackPoints.push_back(0.5); wingBackPoints.push_back(-1.0);

	wingBackPoints.push_back(0.0); wingBackPoints.push_back(-1.0); wingBackPoints.push_back(-1.5);
	wingBackPoints.push_back(0.0); wingBackPoints.push_back(0.0); wingBackPoints.push_back(-1.5);
	wingBackPoints.push_back(0.0); wingBackPoints.push_back(0.5); wingBackPoints.push_back(-1.5);

	wingBack = new Patch(2, 10, 20, "fill", wingBackPoints);

	//Create appearances. If the default textures are not found, warn the user and use just the appearance colors
	float balloonEmissive[4] = {0.0, 0.0, 0.0, 0.0};
	float balloonAmbient[4] = {0.63, 0.63, 0.63, 1.0};
	float balloonDiffuse[4] = {0.63, 0.63, 0.63, 1.0};
	float balloonSpecular[4] = {0.63, 0.63, 0.63, 1.0};
	float balloonShininess = 100.0;
	balloonApp = new Appearance(balloonAmbient, balloonDiffuse, balloonSpecular, balloonEmissive, balloonShininess);
	try {
		balloonApp->setTexture("balloonText.png");
		balloonApp->setTexLength_s(7.0);
		balloonApp->setTexLength_t(5.0);
	}
	catch (GLexception) {
		printf("Zeppelin balloon texture not loaded!\n");
	}

	float cabinSideEmissive[4] = {0.0, 0.0, 0.0, 0.0};
	float cabinSideAmbient[4] = {0.23, 0.23, 0.23, 1.0};
	float cabinSideDiffuse[4] = {0.23, 0.23, 0.23, 1.0};
	float cabinSideSpecular[4] = {0.23, 0.23, 0.23, 1.0};
	float cabinSideShininess = 100.0;
	cabinSideApp = new Appearance(cabinSideAmbient, cabinSideDiffuse, cabinSideSpecular, cabinSideEmissive, cabinSideShininess);
	try {
		cabinSideApp->setTexture("cabinText.png");
		cabinSideApp->setTexLength_s(1.0);
		cabinSideApp->setTexLength_t(1.0);
	}
	catch (GLexception) {
		printf("Zeppelin cabin texture not loaded!\n");
	}
	
	float cabinBottomEmissive[4] = {0.0, 0.0, 0.0, 0.0};
	float cabinBottomAmbient[4] = {0.43, 0.43, 0.43, 1.0};
	float cabinBottomDiffuse[4] = {0.43, 0.43, 0.43, 1.0};
	float cabinBottomSpecular[4] = {0.43, 0.43, 0.43, 1.0};
	float cabinBottomShininess = 100.0;
	cabinBottomApp = new Appearance(cabinBottomAmbient, cabinBottomDiffuse, cabinBottomSpecular, cabinBottomEmissive, cabinBottomShininess);
	try {
		cabinBottomApp->setTexture("cabinBottomText.png");
		cabinBottomApp->setTexLength_s(1.0);
		cabinBottomApp->setTexLength_t(1.0);
	}
	catch (GLexception) {
		printf("Zeppelin cabin bottom texture not loaded!\n");
	}

	float wingEmissive[4] = {0.0, 0.0, 0.0, 0.0};
	float wingAmbient[4] = {0.7, 0.0, 0.0, 1.0};
	float wingDiffuse[4] = {0.4, 0.0, 0.0, 1.0};
	float wingSpecular[4] = {0.1, 0.0, 0.0, 1.0};
	float wingShininess = 50.0;
	wingApp = new Appearance(wingAmbient, wingDiffuse, wingSpecular, wingEmissive, wingShininess);
	try {
		wingApp->setTexture("cabinBottomText.png");
		wingApp->setTexLength_s(1.0);
		wingApp->setTexLength_t(1.0);
	}
	catch (GLexception) {
		printf("Zeppelin wing texture not loaded!\n");
	}
}

Vehicle::~Vehicle() {
	delete balloon;
	delete cabinSide;
	delete cabinBottom;
	delete wingFront;
	delete wingBack;

	delete balloonApp;
	delete cabinSideApp;
	delete cabinBottomApp;
	delete wingApp;
}

void Vehicle::draw() {
	bool hasAppearance = false;
	Appearance* appliedApp;

	if(this->getAppearance() != NULL && this->getAppearance()->id != RootVertex::defaultAppearanceID) {
		hasAppearance = true;
		appliedApp = this->getAppearance();
	}

	//Draw the zeppelin balloon
	glPushMatrix();
	glMultMatrixf(balloonMatrix);
	if(!hasAppearance) {
		appliedApp = balloonApp;
		appliedApp->apply();
	}
	balloon->setAppearance(appliedApp);
	balloon->draw();
	glPopMatrix();

	//Draw the zeppelin cabin sides and bottom
	glPushMatrix();
	if(!hasAppearance) {
		appliedApp = cabinSideApp;
		appliedApp->apply();
	}
	cabinSide->setAppearance(appliedApp);
	cabinSide->draw();
	glRotatef(180, 0.0, 1.0, 0.0);
	cabinSide->draw();
	glPopMatrix();

	if(!hasAppearance) {
		appliedApp = cabinBottomApp;
		appliedApp->apply();
	}
	cabinBottom->setAppearance(appliedApp);
	cabinBottom->draw();

	//Draw the wings
	glPushMatrix();
	if(!hasAppearance) {
		appliedApp = wingApp;
		appliedApp->apply();
	}
	wingFront->setAppearance(appliedApp);
	wingBack->setAppearance(appliedApp);
	glTranslatef(0.0, 1.7, -5.0);
	glTranslatef(0.0, 2.0, 0.0);
	wingFront->draw();
	wingBack->draw();
	glTranslatef(-2.0, -2.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	wingFront->draw();
	wingBack->draw();
	glTranslatef(-2.0, -2.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	wingFront->draw();
	wingBack->draw();
	glTranslatef(-2.0, -2.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);
	wingFront->draw();
	wingBack->draw();
	glPopMatrix();
}
