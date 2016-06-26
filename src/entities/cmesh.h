/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CMESH_H
#define CMESH_H

#include <utility>
#include "cmeshdata.h"
#include "../contexts/graphics/cgraphicscontext.h"
#include "../math/ctransform.h"
#include "../csmartpointer.h"
#include "../physics/cphysics.h"

namespace Citrine
{
	class Mesh
	{
		public:
			Mesh(IGraphicsContext* igraphicscontext, const char* name);
			~Mesh();
			void Update();
			SmartPointer<MeshData> GetMeshData();
			void SetMeshData(SmartPointer<MeshData> meshdata);
			std::pair<Vertex*, unsigned short> GetVertices();
			std::pair<Triangle*, unsigned short> GetTriangles();
			std::pair<Armature*, unsigned short> GetArmatures();
			const void* GetBufferObject();
			void PlayAnimation(const char* name);
			Transform* GetTransformation();
			char* GetName();
			void EnablePhysics(PhysicsWorld* world);
			PhysicsObject* GetPhysics();
		private:
			IGraphicsContext* igraphicscontext;
			SmartPointer<MeshData> meshdata;
			Transform transformation;
			char* name;
			PhysicsObject* phys_object;
			unsigned short current_frame;
			void* current_animation;
			const void* buffer_object;
			bool vbo_initialized;
	};
}
#endif
