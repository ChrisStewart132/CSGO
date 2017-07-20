#pragma once
#include <windows.h>
#include "Structs.h"
#include "Mem.h"
#include <string.h>
#include <cstdlib>
#include <ctime>


class Manager
{
private:
	info* infoPtr;//offsets
	tools* toolsPtr;//settings
	CM* memoryPtr;

	//getTarget
	vect3 targetIndex;
	vect3 targetDistanceArray[12];
	float prevDistance = 0;

	//reticuleProximity
	float x, y, z;
	float distance, angle, verticalAngle;
	vect3 deltaAngle[12];
	int tmp;

	//aimbot
	float recoilFov = 0.0f;
	int shots = 0;

public:
	Manager(info* infoStruct, tools* toolsStruct, CM* memoryClass);
	void update();

	void esp();

	void getTarget();
	vect3 reticuleProximity(int _i);
	void aimbot();

	void lc();
	void trigger();

	~Manager();
};

