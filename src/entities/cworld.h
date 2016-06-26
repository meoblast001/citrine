/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CWORLD_H
#define CWORLD_H

#include <list>
#include "cmesh.h"
#include "csprite3d.h"
#include "clight.h"
#include "ccamera.h"
#include "../csmartpointer.h"
#include "../physics/cphysics.h"

namespace Citrine
{
	class World
	{
		public:
			World(IGraphicsContext* igraphicscontext);
			~World();
			/*Mesh*/
			std::list<SmartPointer<Mesh> >* GetMeshes();
			SmartPointer<Mesh> CreateMesh(const char* name);
			void AddMesh(SmartPointer<Mesh> mesh);
			void RemoveMesh(SmartPointer<Mesh> mesh);
			/*Cameras*/
			std::list<SmartPointer<Camera> >* GetCameras();
			SmartPointer<Camera> CreateCamera();
			void AddCamera(SmartPointer<Camera> camera);
			void RemoveCamera(SmartPointer<Camera> camera);
			/*Sprite3Ds*/
			std::list<SmartPointer<Sprite3D> >* GetSprite3Ds();
			SmartPointer<Sprite3D> CreateSprite3D();
			void AddSprite3D(SmartPointer<Sprite3D> sprite3d);
			void RemoveSprite3D(SmartPointer<Sprite3D> sprite3d);
			/*Lights*/
			std::list<SmartPointer<Light> >* GetLights();
			SmartPointer<Light> CreateLight();
			void AddLight(SmartPointer<Light> light);
			void RemoveLight(SmartPointer<Light> light);
			SmartPointer<Camera> GetCurrentCamera();
			void SetCurrentCamera(SmartPointer<Camera> camera);
			SmartPointer<Mesh> FindMeshByName(const char* name);
			/*Physics*/
			void EnablePhysics();
			PhysicsWorld* GetPhysics();
		private:
			std::list<SmartPointer<Mesh> > meshobjects;
			std::list<SmartPointer<Sprite3D> > sprite3ds;
			std::list<SmartPointer<Light> > lights;
			std::list<SmartPointer<Camera> > cameras;
			SmartPointer<Camera> currentcamera;
			PhysicsWorld* phys_world;
			IGraphicsContext* igraphicscontext;
	};
}
#endif
