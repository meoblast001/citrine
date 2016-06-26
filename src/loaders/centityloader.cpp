/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "centityloader.h"
#include "ccsm.h"
#include "ccse.h"
#include "../cerror.h"

using namespace Citrine;
using namespace ErrorHandling;

inline const char* GetDirectory(const char* filename)
{
	//Determine parent directory
	unsigned int directory_size = strlen(filename) - 1;
	while (filename[directory_size] != '/' && directory_size > 0)
		directory_size--;
	if (directory_size)
	{
		//Add '/' to the copy range
		directory_size++;
		char* directory = (char*) malloc(directory_size + 1);
		strncpy(directory, filename, directory_size);
		//Null termination
		directory[directory_size] = 0;
		return directory;
	}
	return 0;
}

SmartPointer<MeshData> Citrine::IEntityLoader::LoadMeshData(const char* filename, IGraphicsContext* graphics_context)
{
	FILE* file = fopen(filename, "rb");
	//If file handle is null, an error has occurred
	if (!file)
		Error("CSM file \"%s\" not found", filename);

	const char* directory = GetDirectory(filename);

	SmartPointer<MeshData> meshdata = LoadCSM(file, graphics_context, directory);

	//Free directory string if it's valid
	if (directory)
		free((void*) directory);

	return meshdata;
}

SmartPointer<World> Citrine::IEntityLoader::LoadWorld(const char* filename, IGraphicsContext* graphics_context)
{
	FILE* file = fopen(filename, "rb");
	//If file handle is null, an error has occurred
	if (!file)
		Error("CSE file \"%s\" not found", filename);

	const char* directory = GetDirectory(filename);

	SmartPointer<World> world = LoadCSE(file, graphics_context, this, directory);

	//Free directory string if it's valid
	if (directory)
		free((void*) directory);

	return world;
}
