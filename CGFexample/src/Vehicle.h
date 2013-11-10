#ifndef VEHICLE_H
#define VEHICLE_H

#include "RootVertex.h"
#include "ScenePrimitive.h"
#include "Patch.h"

class Vehicle : public ScenePrimitive {
protected:
	Sphere* balloon;
	float balloonMatrix[16];
	Patch* cabinSide;
	Patch* cabinBottom;
	Patch* wingFront;
	Patch* wingBack;

	Appearance* balloonApp;
	Appearance* cabinSideApp;
	Appearance* cabinBottomApp;
	Appearance* wingApp;

public:
	Vehicle();
	~Vehicle();

	void draw();
};

#endif
