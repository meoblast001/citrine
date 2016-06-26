/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <stdlib.h>
#include <string.h>
#include "cmeshdata.h"
#include "../cerror.h"

using namespace Citrine;
using namespace ErrorHandling;

/***************
 MeshData
 ***************/

Citrine::MeshData::MeshData()
{
	texture = 0;
	shader_program = 0;
}

SmartPointer<Texture> Citrine::MeshData::GetTexture()
{
	return texture;
}
void Citrine::MeshData::SetTexture(SmartPointer<Texture> texture)
{
	this->texture = texture;
}
SmartPointer<ShaderProgram> Citrine::MeshData::GetShaderProgram()
{
	return shader_program;
}
void Citrine::MeshData::SetShaderProgram(SmartPointer<ShaderProgram> shader_program)
{
	this->shader_program = shader_program;
}

/***************
 VertexMeshData
 ***************/

Citrine::VertexMeshData::VertexMeshData(IGraphicsContext* igraphicscontext, Frame* base_frame, Animation* animations, unsigned short animations_amount)
{
	this->igraphicscontext = igraphicscontext;
	this->base_frame = base_frame;
	this->animations = animations;
	this->animations_amount = animations_amount;

	Enable();
}

Citrine::VertexMeshData::~VertexMeshData()
{
	Disable();

	//Delete base frame
	delete[] base_frame->vertices;
	delete[] base_frame->triangles;
	delete base_frame;
	//Delete animations
	for (unsigned int i = 0; i < animations_amount; i++)
	{
		free(animations[i].name);
		//Delete frames
		for (unsigned int i2 = 0; i < animations[i].frames_amount; i++)
		{
			delete[] animations[i].frames[i2].vertices;
			delete[] animations[i].frames[i2].triangles;
		}
		delete[] animations[i].frames;
	}
}

void* Citrine::VertexMeshData::GetAnimation(const char* name)
{
	for (unsigned int i = 0; i < animations_amount; i++)
		if (!strcmp(name, animations[i].name))
			return &animations[i];
	return NULL;
}

unsigned short Citrine::VertexMeshData::GetFramesAmount(void* animation)
{
	Animation* _animation = (Animation*) animation;

	if (_animation == NULL)
		return 0;
	return _animation->frames_amount;
}

std::pair<Vertex*, unsigned short> Citrine::VertexMeshData::GetVertices(void* animation, unsigned short frame)
{
	Animation* _animation = (Animation*) animation;

	std::pair<Vertex*, unsigned short> out;
	//Return vertices from base frame if animation is NULL
	if (_animation == NULL)
	{
		out.first = base_frame->vertices;
		out.second = base_frame->vertices_amount;
	}
	//Return vertices from specified frame in the specified animation
	else if (frame < _animation->frames_amount)
	{
		out.first = _animation->frames[frame].vertices;
		out.second = _animation->frames[frame].vertices_amount;
	}
	else
	{
		//Frame does not exist
		out.first = NULL;
		out.second = 0;
	}

	return out;
}

std::pair<Triangle*, unsigned short> Citrine::VertexMeshData::GetTriangles(void* animation, unsigned short frame)
{
	Animation* _animation = (Animation*) animation;

	std::pair<Triangle*, unsigned short> out;
	//Return triangles from base frame if animation is NULL
	if (_animation == NULL)
	{
		out.first = base_frame->triangles;
		out.second = base_frame->triangles_amount;
	}
	//Return triangles from specified frame in the specified animation
	if (frame < _animation->frames_amount)
	{
		out.first = _animation->frames[frame].triangles;
		out.second = _animation->frames[frame].triangles_amount;
	}
	else
	{
		//Frame does not exist
		out.first = NULL;
		out.second = 0;
	}

	return out;
}

std::pair<Armature*, unsigned short> Citrine::VertexMeshData::GetArmatures(void* animation, unsigned short frame)
{
	std::pair<Armature*, unsigned short> out(NULL, 0);
	return out;
}

const void* Citrine::VertexMeshData::GetBufferObject(void* animation, unsigned short frame)
{
	Animation* _animation = (Animation*) animation;

	//Return base frame buffer object if animation is NULL
	if (_animation == NULL)
		return base_frame->buffer_object;
	//Return buffer object from specified frame in specified animation
	if (frame < _animation->frames_amount)
		return _animation->frames[frame].buffer_object;

	//Frame does not exist
	return NULL;
}

void Citrine::VertexMeshData::Enable()
{
	//Enable base frame
	base_frame->buffer_object = igraphicscontext->CreateVBO(base_frame->vertices, base_frame->vertices_amount, base_frame->triangles, base_frame->triangles_amount);
	//Enable each frame in each animation
	for (unsigned int i = 0; i < animations_amount; i++)
	{
		for (unsigned int i2 = 0; i2 < animations[i].frames_amount; i2++)
		{
			Frame* frame = &animations[i].frames[i2];
			for (unsigned int ix = 0; ix < frame->vertices_amount; ix++)
				frame->buffer_object = igraphicscontext->CreateVBO(frame->vertices, frame->vertices_amount, frame->triangles, frame->triangles_amount);
		}
	}
}

void Citrine::VertexMeshData::Disable()
{
	//Disable base frame
	igraphicscontext->DeleteVBO(base_frame->buffer_object);
	//Disable each frame in each animation
	for (unsigned int i = 0; i < animations_amount; i++)
		for (unsigned int i2 = 0; i2 < animations[i].frames_amount; i2++)
			igraphicscontext->DeleteVBO(animations[i].frames[i2].buffer_object);
}

/***************
 ArmatureMeshData
 ***************/

Citrine::ArmatureMeshData::ArmatureMeshData(IGraphicsContext* igraphicscontext, Frame* base_frame, Animation* animations, unsigned short animations_amount, Vertex* vertices, unsigned short vertices_amount, Triangle* triangles, unsigned short triangles_amount)
{
	this->igraphicscontext = igraphicscontext;
	this->base_frame = base_frame;
	this->animations = animations;
	this->animations_amount = animations_amount;
	this->vertices = vertices;
	this->vertices_amount = vertices_amount;
	this->triangles = triangles;
	this->triangles_amount = triangles_amount;

	Enable();
}

Citrine::ArmatureMeshData::~ArmatureMeshData()
{
	Disable();

	//Delete vertices and triangles
	delete[] vertices;
	delete[] triangles;
	//Delete base frame
	delete[] base_frame->armatures;
	delete base_frame;
	//Delete animations
	for (unsigned int i = 0; i < animations_amount; i++)
	{
		free(animations[i].name);
		//Delete frames
		for (unsigned int i2 = 0; i < animations[i].frames_amount; i++)
			delete[] animations[i].frames[i2].armatures;
		delete animations[i].frames;
	}
}

void* Citrine::ArmatureMeshData::GetAnimation(const char* name)
{
	for (unsigned int i = 0; i < animations_amount; i++)
		if (!strcmp(name, animations[i].name))
			return &animations[i];
	return NULL;
}

unsigned short Citrine::ArmatureMeshData::GetFramesAmount(void* animation)
{
	Animation* _animation = (Animation*) animation;

	if (_animation == NULL)
		return 0;
	return _animation->frames_amount;
}

std::pair<Vertex*, unsigned short> Citrine::ArmatureMeshData::GetVertices(void* animation, unsigned short frame)
{
	std::pair<Vertex*, unsigned short> out(vertices, vertices_amount);
	return out;
}

std::pair<Triangle*, unsigned short> Citrine::ArmatureMeshData::GetTriangles(void* animation, unsigned short frame)
{
	std::pair<Triangle*, unsigned short> out(triangles, triangles_amount);
	return out;
}

std::pair<Armature*, unsigned short> Citrine::ArmatureMeshData::GetArmatures(void* animation, unsigned short frame)
{
	Animation* _animation = (Animation*) animation;

	std::pair<Armature*, unsigned short> out;
	//Return armatures from base frame if animation is NULL
	if (_animation == NULL)
	{
		out.first = base_frame->armatures;
		out.second = base_frame->armatures_amount;
	}
	//Return armatures from specified frame in specified animation
	else if (frame < _animation->frames_amount)
	{
		out.first = _animation->frames[frame].armatures;
		out.second = _animation->frames[frame].armatures_amount;
	}
	else
	{
		//Frame does not exist
		out.first = NULL;
		out.second = 0;
	}

	return out;
}

const void* Citrine::ArmatureMeshData::GetBufferObject(void* animation, unsigned short frame)
{
	return buffer_object;
}

void Citrine::ArmatureMeshData::Enable()
{
	buffer_object = igraphicscontext->CreateVBO(vertices, vertices_amount, triangles, triangles_amount);
}

void Citrine::ArmatureMeshData::Disable()
{
	igraphicscontext->DeleteVBO(buffer_object);
}
