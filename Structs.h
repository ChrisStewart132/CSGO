#pragma once
#include <Windows.h>

struct vect3 {
	float x, y, z;

	vect3(float _x, float _y, float _z)
	{
		x = _x, y = _y, z = _z;
	}
	vect3()
	{
		x = 0, y = 0, z = 0;
	}
};

struct info {
	//offsets
	DWORD dwClientState = 0x5A3334;//client state
	DWORD dwClientState_ViewAngles = 0x4D10;

	DWORD localPlayerOffset = 0xAAFFFC;//oPLAYER struct
	DWORD m_dwLocalPlayerIndex = 0x178;
	DWORD crosshairOffset = 0xB2B4;
	DWORD m_shotsFired = 0xA2C0;
	DWORD m_viewPunchAngle = 0x301C;
	DWORD m_vecOrigin = 0x134;

	DWORD entityListOffset = 0x4A8C804;//oENTITYLIST struct
	DWORD m_bDormant = 0xE9;
	DWORD m_iTeamNum = 0xF0;
	DWORD m_iHealth = 0xFC;
	DWORD m_dwBoneMatrix = 0x2698;
	DWORD boneBase;

	DWORD m_dwGlowObject = 0x4FA97F8;//oglowobject manager data
	DWORD dw_GlowIndex = 0xA320;
	DWORD dwForceAttack = 0x2ECCA08;

	//vars
	DWORD client;
	DWORD engine;
	DWORD localPlayer;
	DWORD glowPointer;
	DWORD clientState;
	//updates
	DWORD entity;
	int team, health;
	DWORD glow;
	bool isDormant;

	//player
	int shotsFired;
	int playerTeam;
	vect3 view;//player view angle (aim)
	vect3 viewRecoil;
	vect3 target;
	vect3 recoil = { 0.0f,0.0f,0.0f };
	vect3 oldRecoil = { 0.0f,0.0f,0.0f };
	vect3 playerPos;
	vect3 enemyPos;
	int players = 12;
};

struct tools {
	bool esp = false;
	bool aimbot = false;
	int smooth = 8;
	float fov = 4.0f;
	bool legit = true;	
};

