#include "context.h"

#include <SDL.h>
#include <stdio.h>
#include <string.h>

#include "memory.h"

#define VERTEX_SIZE 9

const GLchar* vertex_shader = "\
#version 130\n \
in vec3 position; \
in vec2 tex_coord; \
in vec4 color; \
uniform mat4 transform; \
out vec4 Color; \
out vec2 Tex_coord; \
void main() { \
	Color = color; \
	Tex_coord = tex_coord; \
	gl_Position = transform * vec4(position, 1.0); \
}";
const GLchar* fragment_shader = "\
#version 130\n \
in vec4 Color; \
in vec2 Tex_coord; \
out vec4 outColor; \
uniform sampler2D tex; \
void main() { \
	outColor = Color * texture(tex, Tex_coord); \
}";

void check_gl_errors() {
	switch (glGetError()) {
		case GL_NO_ERROR:
			return;
		case GL_INVALID_ENUM:
			printf("GL invalid enum\n");
		case GL_OUT_OF_MEMORY:
			printf("GL out of memory\n");
			break;
		case GL_INVALID_VALUE:
			printf("GL invalid value\n");
			break;
		case GL_INVALID_OPERATION:
			printf("GL invalid operation\n");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			printf("GL invalid frambuffer operation");
			break;
	}
	exit(1);
}

AU_Context au_context_init_stack(SDL_Window* wind) {
	AU_Context ctx;
	//Add SDL window and context to the context structure
	ctx.window = wind;
	SDL_GL_SetSwapInterval(1);
	ctx.ctx = SDL_GL_CreateContext(wind);
	//Load OpenGL
	if (!gladLoadGL()) {
		printf("Failed to initialize OpenGL\n");
		exit(1);
	}
	//Set up the texture buffer
	ctx.tex_count = 0;
	ctx.tex_capacity = 32;
	ctx.image_buffer = au_memory_alloc(sizeof(AU_BatchEntry) * ctx.tex_capacity);
	//Create and bind opengl objects
	glGenVertexArrays(1, &ctx.vao);
	glBindVertexArray(ctx.vao);
	glGenBuffers(1, &ctx.vbo);
	glGenBuffers(1, &ctx.ebo);
	//Create and compile shaders
	ctx.vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(ctx.vertex, 1, &vertex_shader, NULL);
	glCompileShader(ctx.vertex);
	GLint status;
	glGetShaderiv(ctx.vertex, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		printf("Vertex shader compilation failed\n");
		char buffer[512];
		glGetShaderInfoLog(ctx.vertex, 512, NULL, buffer);
		printf("Error: %s\n", buffer);
		exit(-1);
	}
	ctx.fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(ctx.fragment, 1, &fragment_shader, NULL);
	glCompileShader(ctx.fragment);
	glGetShaderiv(ctx.fragment, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		printf("Fragment shader compilation failed\n");
		char buffer[512];
		glGetShaderInfoLog(ctx.fragment, 512, NULL, buffer);
		printf("Error: %s\n", buffer);
		exit(-1);
	}
	ctx.shader = glCreateProgram();
	glAttachShader(ctx.shader, ctx.vertex);
	glAttachShader(ctx.shader, ctx.fragment);
	glBindFragDataLocation(ctx.shader, 0, "outColor");
	glLinkProgram(ctx.shader);
	glUseProgram(ctx.shader);
	glEnable (GL_DEPTH_TEST);
	return ctx;
}

AU_Context* au_context_init(SDL_Window* wind) {
	AU_Context* alloc_ctx = au_memory_alloc(sizeof(AU_Context));
	*alloc_ctx = au_context_init_stack(wind);
	return alloc_ctx;
}

void au_context_quit(AU_Context* ctx) {
	for (int i = 0; i < ctx->tex_count; i++) {
		free(ctx->image_buffer[i].vertices);
		free(ctx->image_buffer[i].indices);
	}
	free(ctx->image_buffer);
	glDeleteProgram(ctx->shader);
	glDeleteShader(ctx->fragment);
	glDeleteShader(ctx->vertex);

	glDeleteBuffers(1, &ctx->vbo);
	glDeleteBuffers(1, &ctx->ebo);

	glDeleteVertexArrays(1, &ctx->vao);
	SDL_GL_DeleteContext(ctx->ctx);
}

void au_context_free(AU_Context* ctx) {
	au_context_quit(ctx);
	free(ctx);
}

int au_context_register_texture(AU_Context* ctx, GLuint img) {
	if (ctx->tex_count >= ctx->tex_capacity) {
		ctx->tex_capacity *= 2;
		ctx->image_buffer = au_memory_realloc(ctx->image_buffer, sizeof(AU_BatchEntry) * ctx->tex_capacity);
	}
	AU_BatchEntry* ent = ctx->image_buffer + ctx->tex_count;
	ent->image = img;
	ent->vertex_capacity = 1024;
	ent->vertex_count = 0;
	ent->vertices = au_memory_alloc(sizeof(float) * ent->vertex_capacity * VERTEX_SIZE);
	ent->index_capacity = 64;
	ent->index_count = 0;
	ent->indices = au_memory_alloc(sizeof(int) * ent->index_capacity);
	ctx->tex_count++;
	return ctx->tex_count - 1;
}

int au_context_add_vertex(AU_Context* ctx, int texture,
						  float x, float y, float z, float texX, float texY, float r, float g, float b, float a) {
	AU_BatchEntry* ent = ctx->image_buffer + texture;
	//Reallocate if necessary
	if (ent->vertex_count >= ent->vertex_capacity) {
		ent->vertex_capacity *= 2;
		ent->vertices = au_memory_realloc(ent->vertices, sizeof(float) * ent->vertex_capacity * VERTEX_SIZE);
	}
	//Pack parameters into an array and copy it to the main buffer
	float vertex[] = {x, y, z, texX, texY, r, g, b, a};
	memcpy(ent->vertices + ent->vertex_count * VERTEX_SIZE, vertex, sizeof(float) * VERTEX_SIZE);
	ent->vertex_count++;
	return ent->vertex_count - 1;
}

void au_context_add_index(AU_Context* ctx, int texture, int vertexID) {
	AU_BatchEntry* ent = ctx->image_buffer + texture;
	//Reallocate if necessary
	if (ent->index_count >= ent->index_capacity) {
		ent->index_capacity *= 2;
		ent->indices = au_memory_realloc(ent->indices, sizeof(int) * ent->index_capacity);
	}
	ent->indices[ent->index_count] = vertexID;
	ent->index_count++;
}

void au_context_clear(AU_Context* ctx, AU_Color color) {
	for (int i = 0; i < ctx->tex_count; i++) {
		ctx->image_buffer[i].vertex_count = 0;
		ctx->image_buffer[i].index_count = 0;
	}
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void au_context_present(AU_Context* ctx, AU_Rectangle camera) {
	float left = camera.x, right = camera.x + camera.width, top = camera.y, bottom = camera.y + camera.height;
	float transform_matrix[] = {
		2 / (right - left), 0, 0, 0,
		0, 2 / (top - bottom), 0, 0,
		0, 0, 1, 0,
		-(right + left) / (right - left), -(top + bottom) / (top - bottom), 0, 1
	};
	GLint transform_attrib = glGetUniformLocation(ctx->shader, "transform");
	glUniformMatrix4fv(transform_attrib, 1, GL_FALSE, transform_matrix);
	for (int i = 0; i < ctx->tex_count; i++) {
		AU_BatchEntry* ent = ctx->image_buffer + i;
		//Bind the vertex data
		glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);
		glBufferData(GL_ARRAY_BUFFER, ent->vertex_count * sizeof(float) * VERTEX_SIZE, ent->vertices, GL_STREAM_DRAW);
		//Bind the index data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx->ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ent->index_count * sizeof(GLuint), ent->indices, GL_STREAM_DRAW);
		//Set up the vertex attributes
		GLint posAttrib = glGetAttribLocation(ctx->shader, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
		GLint texAttrib = glGetAttribLocation(ctx->shader, "tex_coord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		GLint colAttrib = glGetAttribLocation(ctx->shader, "color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
		//Upload the texture to the GPU
		ctx->texture_location = glGetUniformLocation(ctx->shader, "tex");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ent->image);
		glUniform1i(ctx->texture_location, 0);
		//Draw the triangles
		glDrawElements(GL_TRIANGLES, ent->index_count, GL_UNSIGNED_INT, 0);
	}
	SDL_GL_SwapWindow(ctx->window);
}
