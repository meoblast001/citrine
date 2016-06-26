/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ccse.h"
#include "../math/cmatrix.h"
#include "../cendianness.h"
#include "../entities/cworld.h"
#include "../math/cquaternion.h"
#include "../cerror.h"

using namespace Citrine;
using namespace Endianness;
using namespace ErrorHandling;

Citrine::World* LoadCSE(FILE* file, Citrine::IGraphicsContext* graphics_context, Citrine::IEntityLoader* entity_loader, const char* directory)
{
	//Construct world instance
	World* world = new World(graphics_context);

	//Header variables
	char ident[9];
	char version_major;
	char version_minor;
	uint32_t content_amount;

	//Get header variables
	fread(ident, 1, 9, file);
	fread(&version_major, 1, 1, file);
	fread(&version_minor, 1, 1, file);
	fread(&content_amount, 1, 4, file);

	//Check identifier
	if (strncmp(ident, "CitrineSE", 9))
		Warning("CSE identifier is not correct");
	//Convert header variables to host endian
	content_amount = ConvertBigEndian(content_amount);

	//Content
	for (unsigned int content_i = 0; content_i < content_amount; content_i++)
	{
		//Content variables
		uint16_t name_size;
		char* name;
		char type;
		float rotation[4];
		float scale[3];
		float translation[3];

		//Get content name
		fread(&name_size, 1, 2, file);
		name_size = ConvertBigEndian(name_size);
		name = (char*) malloc(name_size + 1);
		fread(name, 1, name_size, file);
		//Null terminate
		name[name_size] = 0;
		//Get other content variables
		fread(&type, 1, 1, file);
		fread(rotation, 1, 16, file);
		fread(scale, 1, 12, file);
		fread(translation, 1, 12, file);

		//Convert rotation quaternion to host endian
		for (unsigned int i = 0; i < 4; i++)
			rotation[i] = ConvertBigEndian(rotation[i]);
		//Convert scale to host endian
		for (unsigned int i = 0; i < 3; i++)
			scale[i] = ConvertBigEndian(scale[i]);
		//Convert translation to host endian
		for (unsigned int i = 0; i < 3; i++)
			translation[i] = ConvertBigEndian(translation[i]);

		//Convert rotation quaternion to Matrix3x3
		Transform transform;
		Quaternion q(rotation[0], rotation[1], rotation[2], rotation[3]);
		Matrix3x3 rotation_matrix = q.ConvertToMatrix();
		//Scale rotation matrix
		rotation_matrix.x *= scale[0];
		rotation_matrix.y *= scale[1];
		rotation_matrix.z *= scale[2];

		switch (type)
		{
			//Mesh
			case 1:
			{
				//Mesh variables
				uint16_t path_size;
				char* path;

				//Get path
				fread(&path_size, 1, 2, file);
				path_size = ConvertBigEndian(path_size);
				path = (char*) malloc(path_size + 1);
				fread(path, 1, path_size, file);
				//Null terminate
				path[path_size] = 0;

				//Merge directory and path
				unsigned int directory_size = directory ? strlen(directory) : 0;
				char* fullpath = (char*) malloc(directory_size + strlen(path) + 1);
				if (directory)
					strcpy(fullpath, directory);
				strcpy(fullpath + directory_size, path);
				//Create mesh
				SmartPointer<Mesh> mesh = world->CreateMesh(name);
				mesh->GetTransformation()->SetPosition(translation[0], translation[1], translation[2]);
				mesh->GetTransformation()->SetRotationMatrix(&rotation_matrix);
				SmartPointer<MeshData> meshdata = entity_loader->LoadMeshData(fullpath, graphics_context);
				mesh->SetMeshData(meshdata);

				free(path);
				free(fullpath);
				break;
			}
			//Camera
			case 2:
			{
				SmartPointer<Camera> camera = world->CreateCamera();
				camera->GetTransformation()->SetPosition(translation[0], translation[1], translation[2]);
				camera->GetTransformation()->SetRotationMatrix(&rotation_matrix);
				break;
			}
			//Light
			case 3:
			{
				//Light variables
				bool positioning_type;
				float intensity;
				float color[3];

				//Get light variables
				fread(&positioning_type, 1, 1, file);
				fread(&intensity, 1, 4, file);
				fread(color, 1, 12, file);

				//Convert light variables to host endian
				intensity = ConvertBigEndian(intensity);
				for (unsigned int i = 0; i < 3; i++)
					color[i] = ConvertBigEndian(color[i]);

				//Create light
				SmartPointer<Light> light = world->CreateLight();
				light->GetPosition()->Set(Vector4(translation[0], translation[1], translation[2], positioning_type));
				light->GetAmbient()->Set(Vector4(color[0], color[1], color[2], intensity));
				light->GetDiffuse()->Set(Vector4(color[0], color[1], color[2], intensity));
				light->GetSpecular()->Set(Vector4(color[0], color[1], color[2], intensity));
				break;
			}
		}
		free(name);
	}
	return world;
}
