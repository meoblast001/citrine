/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <stdlib.h>
#include <string.h>
#include "cworld.h"

using namespace Citrine;

Citrine::World::World(IGraphicsContext* igraphicscontext)
{
	this->igraphicscontext = igraphicscontext;
	currentcamera = 0;
	phys_world = 0;
}

Citrine::World::~World()
{
	if (phys_world)
		delete phys_world;
}

std::list<SmartPointer<Mesh> >* Citrine::World::GetMeshes()
{
	return &meshobjects;
}

SmartPointer<Mesh> Citrine::World::CreateMesh(const char* name)
{
	Mesh* mesh = new Mesh(igraphicscontext, name);
	SmartPointer<Mesh> smartmesh = mesh;
	AddMesh(smartmesh);
	return smartmesh;
}

void Citrine::World::AddMesh(SmartPointer<Mesh> mesh)
{
	meshobjects.push_back(mesh);
}

void Citrine::World::RemoveMesh(SmartPointer<Mesh> mesh)
{
	for (std::list<SmartPointer<Mesh> >::iterator i = meshobjects.begin(); i != meshobjects.end(); i++)
	{
		if (**i == *mesh)
		{
			meshobjects.erase(i);
			return;
		}
	}
}

std::list<SmartPointer<Camera> >* Citrine::World::GetCameras()
{
	return &cameras;
}

SmartPointer<Camera> Citrine::World::CreateCamera()
{
	Camera* camera = new Camera;
	SmartPointer<Camera> smartcamera = camera;
	AddCamera(smartcamera);
	return camera;
}

void Citrine::World::AddCamera(SmartPointer<Camera> camera)
{
	cameras.push_back(camera);
}

void Citrine::World::RemoveCamera(SmartPointer<Camera> camera)
{
	for (std::list<SmartPointer<Camera> >::iterator i = cameras.begin(); i != cameras.end(); i++)
	{
		if (**i == *camera)
		{
			cameras.erase(i);
			return;
		}
	}
}

std::list<SmartPointer<Sprite3D> >* Citrine::World::GetSprite3Ds()
{
	return &sprite3ds;
}

SmartPointer<Sprite3D> Citrine::World::CreateSprite3D()
{
	Sprite3D* sprite3d = new Sprite3D;
	SmartPointer<Sprite3D> smartsprite3d = sprite3d;
	AddSprite3D(smartsprite3d);
	return smartsprite3d;
}

void Citrine::World::AddSprite3D(SmartPointer<Sprite3D> sprite3d)
{
	sprite3ds.push_back(sprite3d);
}

void Citrine::World::RemoveSprite3D(SmartPointer<Sprite3D> sprite3d)
{
	for (std::list<SmartPointer<Sprite3D> >::iterator i = sprite3ds.begin(); i != sprite3ds.end(); i++)
	{
		if (**i == *sprite3d)
		{
			sprite3ds.erase(i);
			return;
		}
	}
}

std::list<SmartPointer<Light> >* Citrine::World::GetLights()
{
	return &lights;
}

SmartPointer<Light> Citrine::World::CreateLight()
{
	Light* light = new Light;
	SmartPointer<Light> smartlight = light;
	AddLight(smartlight);
	return smartlight;
}

void Citrine::World::AddLight(SmartPointer<Light> light)
{
	lights.push_back(light);
}

void Citrine::World::RemoveLight(SmartPointer<Light> light)
{
	for (std::list<SmartPointer<Light> >::iterator i = lights.begin(); i != lights.end(); i++)
	{
		if (**i == *light)
		{
			lights.erase(i);
			return;
		}
	}
}

SmartPointer<Mesh> Citrine::World::FindMeshByName(const char* name)
{
	for (std::list<SmartPointer<Mesh> >::iterator i = meshobjects.begin(); i != meshobjects.end(); i++)
	{
		if (!strcmp(name, (*i)->GetName()))
			return *i;
	}
}

SmartPointer<Camera> Citrine::World::GetCurrentCamera()
{
	return currentcamera;
}
void Citrine::World::SetCurrentCamera(SmartPointer<Camera> camera)
{
	currentcamera = camera;
}

void Citrine::World::EnablePhysics()
{
	if (!phys_world)
		phys_world = new PhysicsWorld(this);
}

PhysicsWorld* Citrine::World::GetPhysics()
{
	return phys_world;
}
