#include "Game.h"
#include "Engine.h"
#if defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _DEBUG

int WINAPI wWinMain(_In_ HINSTANCE i_hInstance,
	_In_opt_ HINSTANCE i_hPrevInstance,
	_In_ LPWSTR    i_lpCmdLine,
	_In_ int       i_nCmdShow) {

	if (Engine::Startup()) {
		if (Game::Shutdown()) {
			Game::Startup(i_hInstance, i_hPrevInstance, i_lpCmdLine, i_nCmdShow);
			Game::Run();
			Game::Shutdown();
		}
		Engine::Shutdown();
	}

#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	//system("pause");

	return 0;
}