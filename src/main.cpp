#include <insolence/insolence.h>

#include "render_manager_quads.h"
#include "component_quad.h"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

struct Game : BaseGameWorld
{
	Camera *camera;
	RenderManagerQuads *renderer;

	Entity *quad;

	void Initialise()
	{
		/*
		 * Create a renderer, and set up our Quad system so the
		 * Quad component can be used.
		 */
		renderer = new RenderManagerQuads();
		entity_manager->AddRenderSystem<SystemQuads>(renderer);

		/*
		 * Camera setup for a full screen quad.
		 * This could just be 1x1 - but we might add more quads later.
		 */
		camera = new Camera(GetWindow(), Camera::Type::ORTHO);
		camera->Pan(glm::vec3(-WINDOW_WIDTH / 2.f, -WINDOW_HEIGHT/ 2.f, 0));
		camera->pos.MoveZ(1);

		/* Create our quad and make it the size of the screen. */
		quad = entity_manager->CreateEntity();
		quad->Add<Quad>();
		quad->Get<Transform>()->SetScaleXY(WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	void Update(const GameTime &gametime)
	{
	}

	void Draw()
	{
		renderer->Flush();
	}

	void Unload()
	{
		delete renderer;
		delete camera;
	}
};

int main()
{
	Game *game = new Game();
	game->Run();

	delete game;
	return 0;
}
