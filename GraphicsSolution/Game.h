#pragma once
#include "GLib.h"
#include <windows.h>

struct GameParams {
	HINSTANCE hInstance;
	HINSTANCE hPrevInstance;
	LPWSTR lpCmdLine;
	int nCmdShow;
};

namespace Game
{
	void Startup(_In_ HINSTANCE i_hInstance,
		_In_opt_ HINSTANCE i_hPrevInstance,
		_In_ LPWSTR    i_lpCmdLine,
		_In_ int       i_nCmdShow);
	void Run();
	bool Shutdown();
};




