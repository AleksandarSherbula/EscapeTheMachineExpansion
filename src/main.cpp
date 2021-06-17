#include "Game.h"

int main()
{
#ifndef __EMSCRIPTEN__		
	#ifdef _DEBUG
		ShowWindow(GetConsoleWindow(), SW_SHOW);
	#else
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	#endif
#endif	
	
	if (game->Construct(480, 320, 2, 2, false, true))
		game->Start();
	delete game;
	return 0;
}