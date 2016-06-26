/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ccsm.h"
#include "../cendianness.h"
#include "../entities/cmeshdata.h"
#include "../math/cquaternion.h"
#include "../cerror.h"

using namespace Citrine;
using namespace Endianness;
using namespace ErrorHandling;

static void LoadVertexFrame(FILE* file, VertexMeshData::Frame* frame)
{
	//Frame variables
	uint16_t vertex_amount;
	uint16_t triangle_amount;

	//Get frame variables
	fread(&vertex_amount, 1, 2, file);
	fread(&triangle_amount, 1, 2, file);

	//Convert frame variables to host endian
	vertex_amount = ConvertBigEndian(vertex_amount);
	triangle_amount = ConvertBigEndian(triangle_amount);

	frame->vertices_amount = vertex_amount;
	frame->triangles_amount = triangle_amount;
	frame->vertices = new Vertex[vertex_amount];
	frame->triangles = new Triangle[triangle_amount];

	for (unsigned int vertex_i = 0; vertex_i < vertex_amount; vertex_i++)
	{
		//Vertex variables
		float position[3];
		float normal[3];
		float texcoord[2];

		//Get vertex variables
		fread(position, 1, 12, file);
		fread(normal, 1, 12, file);
		fread(texcoord, 1, 8, file);

		//Convert vertex variables to host endian
		for (unsigned int i = 0; i < 3; i++)
		{
			position[i] = ConvertBigEndian(position[i]);
			normal[i] = ConvertBigEndian(normal[i]);
		}
		for (unsigned int i = 0; i < 2; i++)
			texcoord[i] = ConvertBigEndian(texcoord[i]);

		//Set vertex position
		frame->vertices[vertex_i].position.x = position[0];
		frame->vertices[vertex_i].position.y = position[1];
		frame->vertices[vertex_i].position.z = position[2];
		//Set vertex texture coordinates
		frame->vertices[vertex_i].texcoord.x = texcoord[0];
		frame->vertices[vertex_i].texcoord.y = texcoord[1];
	}

	for (unsigned int triangle_i = 0; triangle_i < triangle_amount; triangle_i++)
	{
		//Triangle variables
		uint16_t vertex_indices[3];

		//Get triangle variables
		fread(vertex_indices, 1, 6, file);

		//Convert triangle variables to host endian
		for (unsigned int i = 0; i < 3; i++)
			vertex_indices[i] = ConvertBigEndian(vertex_indices[i]);

		//Set indices
		frame->triangles[triangle_i].x = vertex_indices[0];
		frame->triangles[triangle_i].y = vertex_indices[1];
		frame->triangles[triangle_i].z = vertex_indices[2];
	}
}

static void LoadVertexCSM(FILE* file, IGraphicsContext* graphics_context, MeshData** meshdata)
{
	//Get base frame
	VertexMeshData::Frame* base_frame = new VertexMeshData::Frame;
	LoadVertexFrame(file, base_frame);

	//Get amount of animations
	uint16_t animation_amount;
	fread(&animation_amount, 1, 2, file);
	animation_amount = ConvertBigEndian(animation_amount);

	//Get animations
	VertexMeshData::Animation* animations = new VertexMeshData::Animation[animation_amount];
	for (unsigned short i = 0; i < animation_amount; i++)
	{
		//Animation variables
		char frame_rate;
		uint16_t frame_amount;

		//Get animation name
		uint16_t animation_name_size;
		fread(&animation_name_size, 1, 2, file);
		animation_name_size = ConvertBigEndian(animation_name_size);
		if (animation_name_size)
		{
			animations[i].name = (char*) malloc(animation_name_size + 1);
			fread(animations[i].name, 1, animation_name_size, file);
			//Null termination
			animations[i].name[animation_name_size] = 0;
		}
		else
			animations[i].name = 0;

		//Get variables
		fread(&frame_rate, 1, 1, file);
		fread(&frame_amount, 1, 2, file);
		frame_amount = ConvertBigEndian(frame_amount);

		//Load frames
		animations[i].frames = new VertexMeshData::Frame[frame_amount];
		for (unsigned short frame_i = 0; frame_i < frame_amount; frame_i++)
			LoadVertexFrame(file, &animations[i].frames[frame_i]);
	}

	//Create new VertexMeshData
	*meshdata = new VertexMeshData(graphics_context, base_frame, animations, animation_amount);
}

static void LoadArmatureFrame(FILE* file, ArmatureMeshData::Frame* frame)
{
	//Get amount of armatures
	fread(&frame->armatures_amount, 1, 1, file);

	frame->armatures = new Armature[frame->armatures_amount];
	for (unsigned char i = 0; i < frame->armatures_amount; i++)
	{
		//Armature variables
		float rotation[4];
		float translation[3];
		unsigned char parent;

		//Read armature variables
		fread(&rotation, 1, 16, file);
		fread(&translation, 1, 12, file);
		fread(&parent, 1, 1, file);

		//Convert endianness
		for (unsigned char rotation_i = 0; rotation_i < 4; rotation_i++)
			rotation[rotation_i] = ConvertBigEndian(rotation[rotation_i]);
		for (unsigned char translation_i = 0; translation_i < 4; translation_i++)
			translation[translation_i] = ConvertBigEndian(translation[translation_i]);

		//Set armature transformation matrix
		Quaternion q(rotation[0], rotation[1], rotation[2], rotation[3]);
		Matrix3x3 rotation_matrix = q.ConvertToMatrix();
		frame->armatures[i].transformation.SetPosition(translation[0], translation[1], translation[2]);
		frame->armatures[i].transformation.SetRotationMatrix(&rotation_matrix);
	}
}

static void LoadArmatureCSM(FILE* file, IGraphicsContext* graphics_context, MeshData** meshdata)
{
	//Get base frame
	ArmatureMeshData::Frame* base_frame = new ArmatureMeshData::Frame;
	LoadArmatureFrame(file, base_frame);

	//Variables
	uint16_t vertices_amount;
	uint16_t triangles_amount;
	uint16_t animation_amount;

	//Get variables
	fread(&vertices_amount, 1, 2, file);
	fread(&triangles_amount, 1, 2, file);
	fread(&animation_amount, 1, 2, file);

	//Convert endianness
	vertices_amount = ConvertBigEndian(vertices_amount);
	triangles_amount = ConvertBigEndian(triangles_amount);
	animation_amount = ConvertBigEndian(animation_amount);

	//Get vertices
	Vertex* vertices = new Vertex[vertices_amount];
	for (unsigned short i = 0; i < vertices_amount; i++)
	{
		//Vertex variables
		float position[3];
		float normal[3];
		float texcoord[2];
		float weights[4];
		unsigned char armatures[4];

		//Read vertex variables
		fread(position, 1, 12, file);
		fread(normal, 1, 12, file);
		fread(texcoord, 1, 8, file);
		fread(weights, 1, 16, file);
		fread(armatures, 1, 4, file);

		//Convert endianness
		for (unsigned char position_i = 0; position_i < 3; position_i++)
			position[position_i] = ConvertBigEndian(position[position_i]);
		for (unsigned char normal_i = 0; normal_i < 3; normal_i++)
			normal[normal_i] = ConvertBigEndian(normal[normal_i]);
		for (unsigned char texcoord_i = 0; texcoord_i < 2; texcoord_i++)
			texcoord[texcoord_i] = ConvertBigEndian(texcoord[texcoord_i]);
		for (unsigned char weights_i = 0; weights_i < 4; weights_i++)
			weights[weights_i] = ConvertBigEndian(weights[weights_i]);

		//Set vertex position
		vertices[i].position.x = position[0];
		vertices[i].position.y = position[1];
		vertices[i].position.z = position[2];
		//Set vertex texture coordinates
		vertices[i].texcoord.x = texcoord[0];
		vertices[i].texcoord.y = texcoord[1];
		//Set vertex weights
		vertices[i].weights.x = weights[0];
		vertices[i].weights.y = weights[1];
		vertices[i].weights.z = weights[2];
		vertices[i].weights.w = weights[3];
		//Set vertex armature indices
		for (unsigned char armatures_i = 0; armatures_i < 4; armatures_i++)
			vertices[i].armatures[armatures_i] = armatures[armatures_i];
	}

	//Get triangles
	Triangle* triangles = new Triangle[triangles_amount];
	for (unsigned int triangle_i = 0; triangle_i < triangles_amount; triangle_i++)
	{
		//Triangle variables
		uint16_t vertex_indices[3];

		//Get triangle variables
		fread(vertex_indices, 1, 6, file);

		//Convert triangle variables to host endian
		for (unsigned int i = 0; i < 3; i++)
			vertex_indices[i] = ConvertBigEndian(vertex_indices[i]);

		//Set indices
		triangles[triangle_i].x = vertex_indices[0];
		triangles[triangle_i].y = vertex_indices[1];
		triangles[triangle_i].z = vertex_indices[2];
	}

	//Get animations
	ArmatureMeshData::Animation* animations = new ArmatureMeshData::Animation[animation_amount];
	for (unsigned short i = 0; i < animation_amount; i++)
	{
		//Animation variables
		uint16_t animation_name_size;
		char* animation_name;
		unsigned char frame_rate;
		uint16_t frame_amount;

		//Get animation name
		fread(&animation_name_size, 1, 2, file);
		animation_name_size = ConvertBigEndian(animation_name_size);
		if (animation_name_size)
		{
			animations[i].name = (char*) malloc(animation_name_size + 1);
			fread(animations[i].name, 1, animation_name_size, file);
			//Null termination
			animations[i].name[animation_name_size] = 0;
		}
		else
			animations[i].name = 0;

		//Get animation variables
		fread(&frame_rate, 1, 1, file);
		fread(&frame_amount, 1, 2, file);
		//Convert endianness
		frame_amount = ConvertBigEndian(frame_amount);

		//Copy values to animation
		animations[i].frame_rate = frame_rate;
		animations[i].frames_amount = frame_amount;

		//Get frames
		animations[i].frames = new ArmatureMeshData::Frame[frame_amount];
		for (unsigned short frame_i = 0; frame_i < frame_amount; frame_i++)
			LoadArmatureFrame(file, &animations[i].frames[frame_i]);
	}

	//Create new ArmatureMeshData
	*meshdata = new ArmatureMeshData(graphics_context, base_frame, animations, animation_amount, vertices, vertices_amount, triangles, triangles_amount);
}

Citrine::MeshData* LoadCSM(FILE* file, Citrine::IGraphicsContext* graphics_context, const char* directory)
{
	//Construct meshdata instance
	MeshData* meshdata;

	//Identification and version variables
	char ident[9];
	char version_major;
	char version_minor;

	//Get identification and version variables
	fread(ident, 1, 9, file);
	fread(&version_major, 1, 1, file);
	fread(&version_minor, 1, 1, file);

	//Check identifier
	if (strncmp(ident, "CitrineSM", 9))
		Warning("CSM identifier is not correct");

	//Animation type variable
	bool animation_type;
	fread(&animation_type, 1, 1, file);

	//Texture Variables
	uint16_t texture_path_size;
	char* texture_path;

	//Get size of texture_path string
	fread(&texture_path_size, 1, 2, file);
	texture_path_size = ConvertBigEndian(texture_path_size);
	//Get texture_path string if texture_path_size isn't 0
	if (texture_path_size)
	{
		//Get texture_path string
		texture_path = (char*) malloc(texture_path_size + 1);
		fread(texture_path, 1, texture_path_size, file);
		//Null termination
		texture_path[texture_path_size] = 0;

		//Load texture
		SmartPointer<Image> image = new Image;
		image->LoadImage(texture_path);
		SmartPointer<Texture> texture = new Texture(graphics_context);
		texture->SetImage(image);
		meshdata->SetTexture(texture);

		free(texture_path);
	}

	if (animation_type)
		LoadArmatureCSM(file, graphics_context, &meshdata);
	else
		LoadVertexCSM(file, graphics_context, &meshdata);

	return meshdata;
}
