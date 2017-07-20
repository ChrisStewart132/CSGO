#include "Mem.h"

CM::CM(HWND _processHandle) {
	hwnd = _processHandle;
	GetWindowThreadProcessId(hwnd, &process);
	handle = OpenProcess(PROCESS_ALL_ACCESS, false, process);
	if (handle) {
		std::cout << "got handle\n";
	}
	else {
		std::cout << "handle error\n";
	}
	Sleep(100);
}

void CM::endCM() {
	CloseHandle(handle);
	Sleep(100);
}

int CM::r(DWORD _address) {
	int s;
	ReadProcessMemory(handle, (LPCVOID)_address, &s, sizeof(s), 0);
	return s;
}

DWORD CM::rDWORD(DWORD _address) {
	DWORD s;
	ReadProcessMemory(handle, (LPCVOID)_address, &s, sizeof(s), 0);
	return s;
}

bool CM::rBool(DWORD _address)
{
	bool s;
	ReadProcessMemory(handle, (LPCVOID)_address, &s, sizeof(s), 0);
	return s;
}

float CM::rFloat(DWORD _address)
{
	float s;
	ReadProcessMemory(handle, (LPCVOID)_address, &s, sizeof(s), 0);
	return s;
}

void CM::wFloat(DWORD _address, float _value)
{
	float buffer = _value;
	bool b = WriteProcessMemory(handle, LPVOID(_address), &buffer, sizeof(buffer), NULL);
}

void CM::wBool(DWORD _address, bool _value)
{
	bool buffer = _value;
	bool b = WriteProcessMemory(handle, LPVOID(_address), &buffer, sizeof(buffer), NULL);
}

void CM::w(DWORD _address, int _value)
{
	int buffer = _value;
	bool b = WriteProcessMemory(handle, LPVOID(_address), &buffer, sizeof(buffer), NULL);
}

DWORD CM::getModule(LPTSTR _module) {
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);
	do
		if (!lstrcmpi(mEntry.szModule, _module))
		{
			CloseHandle(hModule);
			return (DWORD)mEntry.modBaseAddr;
		}
	while (Module32Next(hModule, &mEntry));
	std::cout << "\nerror " << _module << "..\n" << std::endl;
	Sleep(100);
	return 0;
}
