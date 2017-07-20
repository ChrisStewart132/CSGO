#include "windows.h"
#include <iostream>
#include <TlHelp32.h>
#include <string>
#include <math.h>
#include <mmsystem.h>//sound

#include "Structs.h"
#include "Mem.h"
#include "Manager.h"
#pragma comment(lib, "winmm.lib")

float main() {

	info *offset = new info();
	tools *toolsPtr = new tools();

	CM* memoryPtr = nullptr;
	CM CM(FindWindow(NULL, L"Counter-Strike: Global Offensive"));
	memoryPtr = &CM;
	Manager manager(offset, toolsPtr, memoryPtr);

	while (1) {
		manager.update();
		if (GetAsyncKeyState(VK_INSERT)) {
			toolsPtr->esp = !toolsPtr->esp;
			Sleep(300);
			std::cout << "esp: " << toolsPtr->esp << std::endl;
			if (toolsPtr->esp) {
				PlaySound(TEXT("esp.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			else {
				PlaySound(TEXT("!esp.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
		}

		if (GetAsyncKeyState(VK_XBUTTON1)) {
			toolsPtr->aimbot = !toolsPtr->aimbot;
			Sleep(300);
			std::cout << "aimbot: " << toolsPtr->aimbot << std::endl;
			if (toolsPtr->aimbot) {
				PlaySound(TEXT("aim.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			else {
				PlaySound(TEXT("!aim.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
		}

		if (GetAsyncKeyState(VK_ADD)) {
			std::cout << "legit aimbot, adds sleep to aimbot for each smooth, 0/1:false/true?";
			std::cin >> toolsPtr->legit;
			Sleep(300);
			std::cout << "legit: " << toolsPtr->legit << std::endl;
			if (toolsPtr->legit) {
				std::cout << "enter fov: ";
				std::cin >> toolsPtr->fov;
				if (toolsPtr->fov < 0.0f || toolsPtr->fov > 90.0f) {
					toolsPtr->fov = 1.0f;
				}
				std::cout << "fov = +-" << toolsPtr->fov << " degrees\n";

				std::cout << "enter Smooth: ";
				std::cin >> toolsPtr->smooth;
				if (toolsPtr->smooth < 1) {
					toolsPtr->smooth = 1;
				}
				std::cout << "smooth: " << toolsPtr->smooth << "\n";
			}
			else {
				toolsPtr->smooth = 1;
				std::cout << "smooth: " << toolsPtr->smooth << "\n";
				Sleep(300);
				std::cout << "enter new fov: ";
				std::cin >> toolsPtr->fov;
				if (toolsPtr->fov < 0.0f || toolsPtr->fov > 90.0f) {
					toolsPtr->fov = 1.0f;
				}
				std::cout << "new fov = +-" << toolsPtr->fov << " degrees\n";
			}
		}	
		Sleep(2);
	}
	CM.endCM();
	delete offset, toolsPtr, memoryPtr;
	return 0;
}