/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include "cgraphics.h"

struct Citrine::Graphics::_VBO
{
	//If GL VBOs are supported
	GLuint buffers[2];
	//Else
	Vertex* vertices;
	unsigned short n_vertices;
	Triangle* triangles;
	unsigned short n_triangles;
};

struct Citrine::Graphics::_Texture
{
	GLuint texture;
};

Citrine::Graphics::Graphic()
{
	if (already_initialized)
		Error("Citrine::Graphics initialized more than once");
	already_initialized = true;

	//Check if VBOs are supported
	if (glewIsSupported("GL_ARB_vertex_buffer_object"))
		have_vbo = true;
	else
		have_vbo = false;
}

const Citrine::Graphics::VBO Citrine::Graphics::CreateVBO(const Vertex* vertices, unsigned short n_vertices, const Vertex* triangles, unsigned short n_triangles)
{
	_VBO* vbo = new _VBO;

	if (have_vbo)
	{
		//Generate buffers
		glGenBuffersARB (2, vbo->vbos);
		//Buffer vertices
		glBindBufferARB (GL_ARRAY_BUFFER, vbo->vbos[0]);
		glBufferDataARB (GL_ARRAY_BUFFER, n_vertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);
		//Buffer triangle indices
		glBindBufferARB (GL_ELEMENT_ARRAY_BUFFER, vbo->vbos[1]);
		glBufferDataARB (GL_ELEMENT_ARRAY_BUFFER, n_triangles * sizeof(Triangle), triangles, GL_STATIC_DRAW);
	}
	else
	{
		//Copy vertices to VBO
		vbo->vertices = (int*) malloc(n_vertices * sizeof(Vertex));
		memcpy(vbo->vertices, vertices, n_vertices * sizeof(Vertex));
		//Copy triangle indices to VBO
		vbo->texcoords = (float*) malloc(n_triangles * sizeof(Triangle));
		memcpy(vbo->triangles, triangles, ntriangles * sizeof(Triangle));
	}

	return vbo;
}

void Citrine::Graphics::DeleteVBO(const VBO vbo)
{
	if (have_vbo)
		glDeleteBuffersARB(2, vbo->vbos);
	else
	{
		free(vbo->vertices);
		free(vbo->triangles);
	}

	delete vbo;
}

const Texture Citrine::Graphics::CreateTexture(unsigned short width, unsigned short height, const void* pixels)
{
	_Texture* texture = new _Texture;
	glGenTextures(1, &texture->texture);
	glBindTexture(GL_TEXTURE_2D, texture->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return texture;
}

void DeleteTexture(const Texture texture)
{
	glDeleteTextures(1, &texture->textures);
	delete texture;
}

bool Citrine::Graphics::already_initialized = false;
