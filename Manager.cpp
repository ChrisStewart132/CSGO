#include "Manager.h"



Manager::Manager(info* infoStruct, tools* toolsStruct, CM* memoryClass)
{
	infoPtr = infoStruct;
	toolsPtr = toolsStruct;
	memoryPtr = memoryClass;

	infoPtr->client = memoryPtr->getModule(L"client.dll");
	infoPtr->engine = memoryPtr->getModule(L"engine.dll");
	infoPtr->localPlayer = memoryPtr->rDWORD(infoPtr->client + infoPtr->localPlayerOffset);
	infoPtr->glowPointer = memoryPtr->rDWORD(infoPtr->client + infoPtr->m_dwGlowObject);
	infoPtr->clientState = memoryPtr->rDWORD(infoPtr->engine + infoPtr->dwClientState);

	std::cout << "INSERTT for esp,mouse 3 AIMBOT TOGGLE, + to change misc settings\nlegit 8 smooth with 0-10 ms sleep each,4 degree fov\nalt for aimbot trigger/trigger if aimbot off.";
	std::cout << "smooth: " << toolsPtr->smooth << "\nfov: " << toolsPtr->fov << std::endl;
}

void Manager::update()
{
	if (toolsPtr->aimbot) {
		if (GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_MENU) && !GetAsyncKeyState(VK_XBUTTON1)) {
			//player		
			infoPtr->playerTeam = memoryPtr->r(infoPtr->localPlayer + infoPtr->m_iTeamNum);
			infoPtr->shotsFired = memoryPtr->r(infoPtr->localPlayer + infoPtr->m_shotsFired);

			infoPtr->recoil.x = memoryPtr->rFloat(infoPtr->localPlayer + infoPtr->m_viewPunchAngle + 0x0);
			infoPtr->recoil.y = memoryPtr->rFloat(infoPtr->localPlayer + infoPtr->m_viewPunchAngle + 0x4);
			infoPtr->view.x = memoryPtr->rFloat(infoPtr->clientState + infoPtr->dwClientState_ViewAngles + 0x0);
			infoPtr->view.y = memoryPtr->rFloat(infoPtr->clientState + infoPtr->dwClientState_ViewAngles + 0x4);

			infoPtr->playerPos.x = memoryPtr->rFloat(infoPtr->localPlayer + infoPtr->m_vecOrigin + 0x0);
			infoPtr->playerPos.y = memoryPtr->rFloat(infoPtr->localPlayer + infoPtr->m_vecOrigin + 0x4);
			infoPtr->playerPos.z = memoryPtr->rFloat(infoPtr->localPlayer + infoPtr->m_vecOrigin + 0x8);
			//std::cout << infoPtr->view.x << ":" << infoPtr->viewRecoil.x << std::endl;		
			getTarget();//gets closest enemy entity from crosshair in vect3 (degrees)
			aimbot();		
		}
	}
	else {
		trigger();
	}

	if (toolsPtr->esp) {
		infoPtr->playerTeam = memoryPtr->r(infoPtr->localPlayer + infoPtr->m_iTeamNum);
		//entity list, esp
		for (int i = 1; i < infoPtr->players; i++) {
			infoPtr->entity = memoryPtr->r(infoPtr->client + infoPtr->entityListOffset + (i * 0x10));
			infoPtr->isDormant = memoryPtr->rBool(infoPtr->entity + infoPtr->m_bDormant);// && !infoPtr->isDormant
			infoPtr->team = memoryPtr->r(infoPtr->entity + infoPtr->m_iTeamNum);
			infoPtr->glow = memoryPtr->r(infoPtr->entity + infoPtr->dw_GlowIndex);	
			esp();//run glow esp for each entity			
		}
	}
}

void Manager::esp()
{
	if (infoPtr->team != infoPtr->playerTeam && !infoPtr->isDormant) {
		if (!toolsPtr->aimbot) {
			memoryPtr->wFloat(infoPtr->glowPointer + (infoPtr->glow * 0x38 + 0x4), 0.9f);//r
		}
		memoryPtr->wFloat(infoPtr->glowPointer + (infoPtr->glow * 0x38 + 0x8), 0.9f);//g
		memoryPtr->wFloat(infoPtr->glowPointer + (infoPtr->glow * 0x38 + 0xC), 0.5f);//b
		memoryPtr->wFloat(infoPtr->glowPointer + (infoPtr->glow * 0x38 + 0x10), 0.5f);//a
		memoryPtr->wBool(infoPtr->glowPointer + (infoPtr->glow * 0x38 + 0x24), true);//render occluded
		memoryPtr->wBool(infoPtr->glowPointer + (infoPtr->glow * 0x38 + 0x25), false);//render unoccluded
	}
}


void Manager::getTarget()
{
	for (int i = 1; i < infoPtr->players; i++) {
		infoPtr->entity = memoryPtr->r(infoPtr->client + infoPtr->entityListOffset + (i * 0x10));
		infoPtr->team = memoryPtr->r(infoPtr->entity + infoPtr->m_iTeamNum);
		infoPtr->health = memoryPtr->r(infoPtr->entity + infoPtr->m_iHealth);
		infoPtr->isDormant = memoryPtr->rBool(infoPtr->entity + infoPtr->m_bDormant);// && !infoPtr->isDormant
		targetDistanceArray[i] = vect3(699, 699, 699);
		if (infoPtr->team != infoPtr->playerTeam && infoPtr->health > 0 && !infoPtr->isDormant) {//enemy from entity list						
			
			infoPtr->boneBase = memoryPtr->rDWORD(infoPtr->entity + infoPtr->m_dwBoneMatrix);
			infoPtr->enemyPos.x = memoryPtr->rFloat(infoPtr->boneBase + 0x30 * 8 + 0x0c);//get BONE entity position
			infoPtr->enemyPos.y = memoryPtr->rFloat(infoPtr->boneBase + 0x30 * 8 + 0x1c);
			infoPtr->enemyPos.z = memoryPtr->rFloat(infoPtr->boneBase + 0x30 * 8 + 0x2c);

			if (toolsPtr->legit) {
				if (infoPtr->shotsFired % 2 == 0) {//2,4,6,8
					infoPtr->enemyPos.z -= 69.0f;//body
				}
				else {
					infoPtr->enemyPos.z -= 63.0f;//head
				}
				if (infoPtr->shotsFired > 3) {
					infoPtr->enemyPos.z -= 3.0f;//chest after first 5 bullet
				}
			}
			else {
				infoPtr->enemyPos.z -= 64.0f;
			}
			
			targetDistanceArray[i] = reticuleProximity(i);//get distance of enemy from reticule in degrees
		}
	}
	for (int i = 1; i < infoPtr->players; i++) {//get closest target to reticule (targetIndex)
		if (i == 1) {
			prevDistance = targetDistanceArray[i].x + targetDistanceArray[i].y;
			targetIndex = vect3(targetDistanceArray[i].x, targetDistanceArray[i].y, i);
		}
		else if ((targetDistanceArray[i].x + targetDistanceArray[i].y) < prevDistance) {
			prevDistance = targetDistanceArray[i].x + targetDistanceArray[i].y;
			targetIndex = vect3(targetDistanceArray[i].x, targetDistanceArray[i].y, i);;
		}
	}
	//std::cout << "x: " << targetIndex.x << ", y: " << targetIndex.y << ", index: " << targetIndex.z << std::endl;
	//int po = targetIndex.z;
	//std::cout << "x: " << deltaAngle[po].x << ", y: " << deltaAngle[po].y << ", : " << infoPtr->view.y << std::endl;
}

vect3 Manager::reticuleProximity(int _i)
{
	if (infoPtr->enemyPos.y < infoPtr->playerPos.y && infoPtr->view.y > -180.0f && infoPtr->view.y < -0.0f) {
		//if enemy closer to t spawn then player (NORTH), and player facing north
		x = infoPtr->enemyPos.x - infoPtr->playerPos.x;
		y = infoPtr->enemyPos.y - infoPtr->playerPos.y;
		z = infoPtr->enemyPos.z - infoPtr->playerPos.z;
		distance = sqrt((x*x) + (y*y));
		angle = (atan(x / y)) * (180.0f / 3.141592);
		deltaAngle[_i].y = angle + infoPtr->view.y + 90;//- turn left + turn right; TODO use recoiled view for reticule proximity
		//std::cout << "a: " << angle << ",v.y: " << infoPtr->view.y << "," << deltaAngle[_i].y << std::endl;
		verticalAngle = (atan(z / distance)) * (180.0f / 3.141592);
		deltaAngle[_i].x = verticalAngle + infoPtr->view.x;//+ aim up, - aim down; TODO use recoiled view for reticule proximity
		//std::cout << "a: " << verticalAngle << ",v.y: " << infoPtr->view.x << "," << deltaAngle[_i].x << std::endl;
		//return magnitude of x and y degrees from target
		if (deltaAngle[_i].x < 0 && deltaAngle[_i].y < 0) {
			return vect3((-1.0f * deltaAngle[_i].x), (-1.0f * deltaAngle[_i].y), 0.0f);
		}
		else if (deltaAngle[_i].x < 0 || deltaAngle[_i].y < 0) {
			if (deltaAngle[_i].x < 0) {
				return vect3((-1.0f * deltaAngle[_i].x), deltaAngle[_i].y, 0.0f);
			}
			else {
				return vect3(deltaAngle[_i].x, (-1.0f * deltaAngle[_i].y), 0.0f);
			}
		}
		else {
			return vect3(deltaAngle[_i].x, deltaAngle[_i].y, 0.0f);
		}
	}
	else if (infoPtr->enemyPos.y > infoPtr->playerPos.y && infoPtr->view.y < 180.0f && infoPtr->view.y > 0.0f) {
		//if enemy closer to ct spawn then player (SOUTH), and player facing south
		x = infoPtr->enemyPos.x - infoPtr->playerPos.x;
		y = infoPtr->enemyPos.y - infoPtr->playerPos.y;
		z = infoPtr->enemyPos.z - infoPtr->playerPos.z;
		distance = sqrt((x*x) + (y*y));
		angle = (atan(x / y)) * (180.0f / 3.141592);
		deltaAngle[_i].y = angle + infoPtr->view.y - 90;//- turn left + turn right; TODO use recoiled view for reticule proximity
		//std::cout << "a: " << angle << ",v.y: " << infoPtr->view.y << "," << deltaAngle[_i].y << std::endl;
		verticalAngle = (atan(z / distance)) * (180.0f / 3.141592);
		deltaAngle[_i].x = verticalAngle + infoPtr->view.x;//+ aim up, - aim down; TODO use recoiled view for reticule proximity
		//std::cout << "a: " << verticalAngle << ",v.y: " << infoPtr->view.x << "," << deltaAngle[_i].x << std::endl;
		//return magnitude of x and y degrees from target
		if (deltaAngle[_i].x < 0 && deltaAngle[_i].y < 0) {
			return vect3((-1.0f * deltaAngle[_i].x), (-1.0f * deltaAngle[_i].y), 0.0f);
		}
		else if (deltaAngle[_i].x < 0 || deltaAngle[_i].y < 0) {
			if (deltaAngle[_i].x < 0) {
				return vect3((-1.0f * deltaAngle[_i].x), deltaAngle[_i].y, 0.0f);
			}
			else {
				return vect3(deltaAngle[_i].x, (-1.0f * deltaAngle[_i].y), 0.0f);
			}
		}
		else {
			return vect3(deltaAngle[_i].x, deltaAngle[_i].y, 0.0f);
		}
	}
	else {
		return vect3(300.0f, 300.0f, 300.0f);
	}

}

void Manager::aimbot()
{
	tmp = targetIndex.z;
	if (targetIndex.x < toolsPtr->fov && targetIndex.y < toolsPtr->fov) {//target is within fov,	and spotted?
		if (infoPtr->shotsFired > 1) {
			for (int i = 0; i < toolsPtr->smooth; i++) {
				infoPtr->view.x -= (deltaAngle[tmp].x + (2 * infoPtr->recoil.x)) / toolsPtr->smooth;
				infoPtr->view.y -= (deltaAngle[tmp].y + (2 * infoPtr->recoil.y)) / toolsPtr->smooth;
				memoryPtr->wFloat(infoPtr->clientState + infoPtr->dwClientState_ViewAngles + 0x0, infoPtr->view.x);
				memoryPtr->wFloat(infoPtr->clientState + infoPtr->dwClientState_ViewAngles + 0x4, infoPtr->view.y);
				if (toolsPtr->legit) {
					Sleep(toolsPtr->smooth - i);
				}
			}
		}
		else {
			for (int i = 0; i < toolsPtr->smooth; i++) {
				infoPtr->view.x -= deltaAngle[tmp].x / toolsPtr->smooth;
				infoPtr->view.y -= deltaAngle[tmp].y / toolsPtr->smooth;
				memoryPtr->wFloat(infoPtr->clientState + infoPtr->dwClientState_ViewAngles + 0x0, infoPtr->view.x);
				memoryPtr->wFloat(infoPtr->clientState + infoPtr->dwClientState_ViewAngles + 0x4, infoPtr->view.y);
				if (toolsPtr->legit) {
					Sleep(toolsPtr->smooth - i);
				}
			}
		}
		trigger();

		infoPtr->entity = memoryPtr->r(infoPtr->client + infoPtr->entityListOffset + (tmp * 0x10));
		infoPtr->health = memoryPtr->r(infoPtr->entity + infoPtr->m_iHealth);
		if (toolsPtr->legit && infoPtr->health < 1) {
			Sleep(800);//target died, sleep aimbot
		}
	}
}

void Manager::lc()
{
	INPUT    Input = { 0 };
	// left down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));

	Sleep(1);

	// left up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}

void Manager::trigger() {
	if (GetAsyncKeyState(VK_MENU)) {
		infoPtr->localPlayer = memoryPtr->rDWORD(infoPtr->client + infoPtr->localPlayerOffset);
		if (memoryPtr->r(infoPtr->localPlayer + infoPtr->crosshairOffset) != 0) {
			lc();
		}
	}
}


Manager::~Manager()
{
}
