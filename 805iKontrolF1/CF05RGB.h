#pragma once
#pragma comment(lib, "CF05RGB.lib")

//This library uses the Windows HID API to communicate with the RGB LED Controller
//included with the IN-WIN 805 Infinity Chassis.

namespace CF05RGB
{
	__declspec(dllimport) int Red;
	__declspec(dllimport) int Green;
	__declspec(dllimport) int Blue;
	__declspec(dllimport) int Brightness;

	__declspec(dllexport) bool Setup(); //These should only be called
	__declspec(dllexport) bool Cleanup(); //if Update() returns <= 0
	__declspec(dllimport) int Update();
}