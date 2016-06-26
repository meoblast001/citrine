/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <stdlib.h>
#include <string.h>
#include "cmesh.h"

using namespace Citrine;

Citrine::Mesh::Mesh(IGraphicsContext* igraphicscontext, const char* name)
{
	this->igraphicscontext = igraphicscontext;
	this->name = (char*) malloc(strlen(name) + 1);
	strcpy(this->name, name);
	phys_object = 0;
	current_frame = 0;
	current_animation = NULL;
	buffer_object = NULL;
	vbo_initialized = false;
}

Citrine::Mesh::~Mesh()
{
	free(name);
	if (phys_object)
		delete phys_object;
	if (buffer_object)
		igraphicscontext->DeleteVBO(buffer_object);
}

void Citrine::Mesh::Update()
{
	//Advance frame
	current_frame++;
	//Revert if new frame is invalid
	if (current_frame >= meshdata->GetFramesAmount(current_animation))
		current_frame--;

	//Animate armatures if an armature animation is being played
	if (current_animation)
	{
		std::pair<Armature*, unsigned short> armatures = meshdata->GetArmatures(current_animation, current_frame);
		if (armatures.first)
		{
			//Initialize animation VBO if uninitialized
			if (!vbo_initialized)
			{
				buffer_object = igraphicscontext->CreateVBO();
				vbo_initialized = true;
			}
			//Deform vertices, get triangles, and rebuffer animation VBO
			std::pair<Vertex*, unsigned short> vertices = ArmatureVertexDeform(meshdata->GetVertices(current_animation, current_frame), armatures);
			std::pair<Triangle*, unsigned short> triangles = meshdata->GetTriangles(current_animation, current_frame);
			igraphicscontext->RebufferVBO(buffer_object, vertices.first, vertices.second, triangles.first, triangles.second);
		}
	}
}

SmartPointer<MeshData> Citrine::Mesh::GetMeshData()
{
	return meshdata;
}

void Citrine::Mesh::SetMeshData(SmartPointer<MeshData> meshdata)
{
	this->meshdata = meshdata;
}

std::pair<Vertex*, unsigned short> Citrine::Mesh::GetVertices()
{
	return meshdata->GetVertices(current_animation, current_frame);
}

std::pair<Triangle*, unsigned short> Citrine::Mesh::GetTriangles()
{
	return meshdata->GetTriangles(current_animation, current_frame);
}

std::pair<Armature*, unsigned short> Citrine::Mesh::GetArmatures()
{
	return meshdata->GetArmatures(current_animation, current_frame);
}

const void* Citrine::Mesh::GetBufferObject()
{
	return buffer_object && current_animation ? buffer_object : meshdata->GetBufferObject(current_animation, current_frame);
}

void Citrine::Mesh::PlayAnimation(const char* name)
{
	current_animation = meshdata->GetAnimation(name);
}

Transform* Citrine::Mesh::GetTransformation()
{
	return &transformation;
}

char* Citrine::Mesh::GetName()
{
	return name;
}

void Citrine::Mesh::EnablePhysics(PhysicsWorld* world)
{
	if (!phys_object)
		phys_object = new PhysicsObject(this, world);
}

PhysicsObject* Citrine::Mesh::GetPhysics()
{
	return phys_object;
}
