/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CENTITYLOADER_H
#define CENTITYLOADER_H

#include "../csmartpointer.h"
#include "../entities/cmeshdata.h"
#include "../entities/cworld.h"

namespace Citrine
{
	class IEntityLoader
	{
		public:
			SmartPointer<MeshData> LoadMeshData(const char* filename, IGraphicsContext* graphics_context);
			SmartPointer<World> LoadWorld(const char* filename, IGraphicsContext* graphics_context);
	};
}
#endif
