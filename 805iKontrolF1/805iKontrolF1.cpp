#include "805iKontrolF1.h"

static HMIDIIN KontrolF1 = nullptr;
static bool hMidiInOpen = false;
static bool CF05RGBOpen = true;

void GetDevice()
{
	std::cout << "Refreshing Device List...\n";
	int nDevs = midiInGetNumDevs();
	MIDIINCAPS inDevCaps;
	MMRESULT retval = MMSYSERR_NOERROR;

	for (int i = 0; i < nDevs; ++i)
	{
		midiInGetDevCaps(i, &inDevCaps, sizeof(inDevCaps));
		std::cout << inDevCaps.szPname << "\n";
		if (strstr(inDevCaps.szPname, MIDI_DEV_NAME))
		{
			retval = midiInOpen(&KontrolF1, i, (DWORD_PTR)&MidiInProc, NULL,
				CALLBACK_FUNCTION | MIDI_IO_STATUS);
			if (retval != MMSYSERR_NOERROR)
			{
				std::cout << "Failed to Open Device\n";
				ReleaseDevice();
				return;
			}
			break;
		}
	}

	retval = midiInStart(KontrolF1);
	if (retval != MMSYSERR_NOERROR)
	{
		std::cout << "Failed to Start Device\n";
		ReleaseDevice();
	}
}

void ReleaseDevice()
{
	midiInStop(KontrolF1);
	midiInClose(KontrolF1);
	KontrolF1 = nullptr;
}

void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance,
	DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	int status = 0;
	int ctrlID = 0;
	int value = 0;
	int retval = 0;

	switch (wMsg)
	{
		default: std::cout << "Unknown Message\n"; break;
		case(MIM_OPEN):
			std::cout << "Device Opened\n";
			hMidiInOpen = true;
			break;
		case(MIM_CLOSE):
			std::cout << "Device Closed\n";
			hMidiInOpen = false;
		break;
		case(MIM_DATA):
		case(MIM_MOREDATA):
			status = LOBYTE(LOWORD(dwParam1));
			ctrlID = HIBYTE(LOWORD(dwParam1));
			value = LOBYTE(HIWORD(dwParam1));
			std::cout << "Data: " << status << ", " << ctrlID << ", " << value << "\n";
			if (status == 188)
			{
				if (ctrlID == 6) CF05RGB::Red = value * 255 / 127;
				if (ctrlID == 7) CF05RGB::Green = value * 255 / 127;
				if (ctrlID == 8) CF05RGB::Blue = value * 255 / 127;
				if (ctrlID == 9) CF05RGB::Brightness = value * 255 / 127;
				retval = CF05RGB::Update();
				if (retval <= 0) CF05RGBOpen = false;
			}
		break;
		case(MIM_LONGDATA):
			std::cout << "SysEx Message\n";
			hMidiInOpen = false;
		break;
		case(MIM_ERROR):
			std::cout << "Device Error\n";
			hMidiInOpen = false;
		break;
		case(MIM_LONGERROR):
			std::cout << "Device Long Error\n";
			hMidiInOpen = false;
		break;
	}
}

void app(bool& running)
{
	CF05RGB::Red = 0;
	CF05RGB::Green = 0;
	CF05RGB::Blue = 255;
	CF05RGB::Brightness = 255;
	CF05RGB::Update();

	int numDevs = 0;
	int numDevsPrev = -1;

	while (running)
	{
		numDevs = midiInGetNumDevs();
		if ((numDevs != numDevsPrev)
		||  (!hMidiInOpen)
		||  (!KontrolF1))
		{
			ReleaseDevice();
			GetDevice();
		}
		numDevsPrev = numDevs;

		if (!CF05RGBOpen)
		{
			std::cout << "Error Recovery Taking Place...\n";
			int values[4] =
			{
				CF05RGB::Red,
				CF05RGB::Green,
				CF05RGB::Blue,
				CF05RGB::Brightness
			};

			if (CF05RGB::Setup())
			{
				CF05RGB::Red = values[0];
				CF05RGB::Green = values[1];
				CF05RGB::Blue = values[2];
				CF05RGB::Brightness = values[3];
				if (CF05RGB::Update() > 0) CF05RGBOpen = true;
				else std::cout << "Error Recovery Failed\n";
			}
			else std::cout << "Error Recovery Failed\n";
		}

		Sleep(100);
	}

	CF05RGB::Red = 0;
	CF05RGB::Green = 0;
	CF05RGB::Blue = 0;
	CF05RGB::Brightness = 0;
	CF05RGB::Update();

	ReleaseDevice();
}