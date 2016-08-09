#include "render_manager_quads.h"
#include <insolence/game/camera.h>

RenderManagerQuads::RenderManagerQuads()
{
	float verts[] = {
		-0.5f,  0.5f,      // Top left
		 0.5f,  0.5f,      // Top right
		 0.5f, -0.5f,      // Bottom right
		 0.5f, -0.5f,      // Bottom right
		-0.5f, -0.5f,      // Bottom left
		-0.5f,  0.5f,      // Top left
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo_verts);
	glGenBuffers(1, &vbo_data);

	vert_shader = Shader::CreateFromFile("assets/quad_shader.vs",
			GL_VERTEX_SHADER, ShaderFlags::ADD_HEADER);
	frag_shader = Shader::CreateFromFile("assets/quad_shader.fs",
			GL_FRAGMENT_SHADER, ShaderFlags::ADD_HEADER);

	if(vert_shader->GetCompileStatus() != GL_TRUE)
		vert_shader->LogCompileInfo("RenderManagerQuads: Vertex ");

	if(frag_shader->GetCompileStatus() != GL_TRUE)
		frag_shader->LogCompileInfo("RenderManagerQuads: Vertex ");

	shader_program = new ShaderProgram();
	glAttachShader(shader_program->GetID(), vert_shader->GetID());
	glAttachShader(shader_program->GetID(), frag_shader->GetID());
	glLinkProgram(shader_program->GetID());
	glUseProgram(shader_program->GetID());

	/* Base quad attributes. */
	glBindBuffer(GL_ARRAY_BUFFER, vbo_verts);
	attrib_verts = glGetAttribLocation(shader_program->GetID(), "in_verts");

	glVertexAttribPointer(attrib_verts, 2, GL_FLOAT, GL_FALSE,
			2 * sizeof(GL_FLOAT), 0);

	glVertexAttribDivisor(attrib_verts, 0);
	glEnableVertexAttribArray(attrib_verts);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STREAM_DRAW);

	/* Individual quad attributes. */
	glBindBuffer(GL_ARRAY_BUFFER, vbo_data);
	attrib_pos = glGetAttribLocation(shader_program->GetID(), "in_pos");
	attrib_scale = glGetAttribLocation(shader_program->GetID(), "in_scale");

	glVertexAttribPointer(attrib_pos, 2, GL_FLOAT, GL_FALSE,
			4 * sizeof(GL_FLOAT), (GLvoid*)0);
	glVertexAttribPointer(attrib_scale, 2, GL_FLOAT, GL_FALSE,
			4 * sizeof(GL_FLOAT), (GLvoid*)(2 * sizeof(GLfloat)));

	glVertexAttribDivisor(attrib_pos, 1);
	glVertexAttribDivisor(attrib_scale, 1);
	glEnableVertexAttribArray(attrib_pos);
	glEnableVertexAttribArray(attrib_scale);

	uni_time = glGetUniformLocation(shader_program->GetID(), "elapsed");
	Camera::Setup(shader_program);
}

void RenderManagerQuads::ReplaceFragShader(Shader *f)
{
	if(frag_shader != NULL)
		glDetachShader(shader_program->GetID(), frag_shader->GetID());

	glAttachShader(shader_program->GetID(), f->GetID());
	glLinkProgram(shader_program->GetID());
	glUseProgram(shader_program->GetID());
	Camera::Setup(shader_program);
	uni_time = glGetUniformLocation(shader_program->GetID(), "elapsed");

	if(frag_shader != NULL)
		delete frag_shader;
	frag_shader = f;
}

RenderManagerQuads::~RenderManagerQuads()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo_verts);
	glDeleteBuffers(1, &vbo_data);
	glDeleteVertexArrays(1, &vao);

	delete shader_program;
	delete vert_shader;
	delete frag_shader;
}

void RenderManagerQuads::Add(const glm::vec2 &pos, const glm::vec2 &scale)
{
	/*
	 * Note: this kind of constant allocation is a bad thing. To do it
	 * properly, we'd appropriately preallocate the data vector.
	 */
	Layout *l;
	data.push_back(Layout());

	l = &data.back();
	l->pos = pos;
	l->scale = scale;
}

void RenderManagerQuads::Flush()
{
	glUseProgram(shader_program->GetID());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1f(uni_time, ++frame_counter);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_verts);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_data);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size() * 4,
			&data[0], GL_DYNAMIC_DRAW);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, data.size());

	data.clear();
}
