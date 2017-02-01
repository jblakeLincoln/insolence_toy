#ifndef INSOLENCE_TOY_GAME_H
#define INSOLENCE_TOY_GAME_H

#include <insolence/insolence.h>
#include <string>
#include "render_manager_quads.h"
#include "component_quad.h"

struct Game : BaseGameWorld
{
	RenderManagerQuads *renderer;
	Camera *camera;
	Entity *quad;

	void Initialise();
	void Update(const GameTime &gametime);
	void Draw(const GameTime &gametime);
	void Unload();

	std::string GetQuadFragShader();
	std::string SetQuadFragShader(const std::string &str);
};


#endif
