#ifndef RENDER_MANAGER_QUAD_H
#define RENDER_MANAGER_QUAD_H

#include <insolence/render/render_manager.h>
#include <vector>

struct RenderManagerQuads
{
	ShaderProgram *shader_program;
	Shader *vert_shader;
	Shader *frag_shader;

	struct Layout {
		glm::vec2 pos;
		glm::vec2 scale;
	};

	GLuint vao;

	GLuint vbo_verts;
	GLint attrib_verts;
	GLint attrib_uvs;

	GLuint vbo_data;
	GLint attrib_pos;
	GLint attrib_scale;

	std::vector<Layout> data;

	int frame_counter = 0;
	GLint uni_time;

	RenderManagerQuads();
	~RenderManagerQuads();

	void Add(const glm::vec2 &pos, const glm::vec2 &scale);
	void ReplaceFragShader(Shader*);
	void Flush(const GameTime&);
};

#endif
