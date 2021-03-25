#include "805iKontrolF1.h"

//Documentation: https://msdn.microsoft.com/en-us/library/windows/desktop/bb540475(v=vs.85).aspx

static bool running = true;
static SERVICE_STATUS_HANDLE Service = nullptr;

int main(int argc, char* argv[])
{
	SetConsoleCtrlHandler(ExitHandler, TRUE);
	
	if ((argc > 1) && (strcmp(argv[1], "service") == 0))
	{
		SERVICE_TABLE_ENTRY entryPoints[] =
		{
			{ SERVICE_NAME, ServiceMain },
			{ NULL, NULL }
		};

		StartServiceCtrlDispatcher(entryPoints);
	}
	else app(running);

	return 0;
}

BOOL WINAPI ExitHandler(DWORD signal)
{
	running = false;
	return TRUE;
}

void ServiceReport(DWORD state)
{
	SERVICE_STATUS status;
	status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	status.dwCurrentState = state;
	SetServiceStatus(Service, &status);
}

VOID WINAPI ServiceProc(DWORD fdwControl)
{
	switch (fdwControl)
	{
		default: break;
		case(SERVICE_CONTROL_STOP):
			ServiceReport(SERVICE_STOP_PENDING);
			running = false;
		break;
	}
}

VOID WINAPI ServiceMain(DWORD argc, LPTSTR* argv)
{
	Service = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceProc);
	if (!Service) return;

	ServiceReport(SERVICE_RUNNING);
	app(running);
	ServiceReport(SERVICE_STOPPED);
}