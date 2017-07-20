#pragma once
#include "windows.h"
#include <iostream>
#include <TlHelp32.h>

class CM
{
private:
	HWND hwnd = NULL;
	HANDLE handle = NULL;
	DWORD process = NULL;
public:
	CM(HWND _processhandle);
	int r(DWORD _address);
	DWORD rDWORD(DWORD _address);
	bool rBool(DWORD _address);
	float rFloat(DWORD _address);
	void wFloat(DWORD _address, float _value);
	void wBool(DWORD _address, bool _value);
	void w(DWORD _address, int _value);
	DWORD getModule(LPTSTR _module);
	void endCM();
};

