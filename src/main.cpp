#include <insolence/insolence.h>

#include "game.h"

#ifdef INSOLENCE_WEBGL
#include <emscripten/bind.h>
#endif

Game *game;
int main()
{
	game = new Game();
	game->Run(640, 360);

	delete game;
	return 0;
}

#ifdef INSOLENCE_WEBGL
std::string web_SetFragShader(std::string str)
{
	return game->SetQuadFragShader(str);
}

std::string web_GetFragShader()
{
	return game->GetQuadFragShader();
}

EMSCRIPTEN_BINDINGS(module)
{
	emscripten::function("getFragShader", &web_GetFragShader);
	emscripten::function("setFragShader", &web_SetFragShader);
}
#endif
