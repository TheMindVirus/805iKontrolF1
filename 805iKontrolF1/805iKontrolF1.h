#pragma once

#include <iostream>
#include <Windows.h>
#include <mmsystem.h>
#include "805iKontrolF1.h"
#include "CF05RGB.h"
#pragma comment(lib, "WinMM.lib")
#pragma comment(lib, "CF05RGB.lib")

#define MIDI_DEV_NAME "Traktor Kontrol F1"
#define SERVICE_NAME "805iKontrolF1"

void GetDevice();
void ReleaseDevice();
void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance,
	DWORD_PTR dwParam1, DWORD_PTR dwParam2);
void app(bool& running);

int main(int argc, char* argv[]);
BOOL WINAPI ExitHandler(DWORD signal);
VOID WINAPI ServiceProc(DWORD fdwControl);
VOID WINAPI ServiceMain(DWORD argc, LPTSTR* argv);