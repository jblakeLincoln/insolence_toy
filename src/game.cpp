#include "game.h"

void Game::Initialise()
{
	/* Insolence TODO: GetWindowDimensions. */
	glm::vec2 window_dimensions = glm::vec2(GetWindow()->GetFramebufferWidth(),
			GetWindow()->GetFramebufferHeight());
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
	camera->Pan(glm::vec3(
				-window_dimensions.x / 2.f,
				-window_dimensions.y / 2.f,
				0));
	camera->pos.MoveZ(1);

	/* Create our quad and make it the size of the screen. */
	quad = entity_manager->CreateEntity();
	quad->Add<Quad>();
	quad->Get<Transform>()->SetScaleXY(window_dimensions.x,
			window_dimensions.y);
}

void Game::Update(const GameTime &gametime)
{

}

void Game::Draw()
{
	renderer->Flush();
}

void Game::Unload()
{
	delete renderer;
	delete camera;
}

std::string Game::GetQuadFragShader()
{
	std::string str;
	char *buf;
	FILE *file = fopen("assets/quad_shader.fs", "r");
	int length;

	if(file == NULL)
		return "ERROR";

	fseek(file, 0, SEEK_END);
	length = ftell(file);
	rewind(file);
	buf = new char[length+1];
	buf[length] = '\0';
	int temp = fread(buf, 1, length, file);
	fclose(file);

	str = buf;
	free(buf);

	return str;
}

std::string Game::SetQuadFragShader(const std::string &str)
{
	Shader *temp = Shader::CreateFromString(str.c_str(),
			GL_FRAGMENT_SHADER, ShaderFlags::ADD_HEADER);

	if(temp->GetShaderiv(GL_COMPILE_STATUS) != GL_TRUE)
		return "Shader compilation unsuccessful!";

	ShaderProgram *program = renderer->shader_program;

	glDetachShader(program->GetID(), renderer->frag_shader->GetID());
	glAttachShader(program->GetID(), temp->GetID());

	delete renderer->frag_shader;
	renderer->frag_shader = temp;

	glLinkProgram(program->GetID());
	glUseProgram(program->GetID());
	Camera::Setup(program);

	return "Shader compilation successful: \n" + str;
}
