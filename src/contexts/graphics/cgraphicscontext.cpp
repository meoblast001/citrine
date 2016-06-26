/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <stdlib.h>
#include "cgraphicscontext.h"
#include "cgraphics.h"
#include "../../entities/cmeshdata.h"
#include "../../entities/csprite2d.h"
#include "../../shaders/cshader.h"
#include "../../shaders/cshaderprogram.h"
#include "../../entities/cworld.h"
#include "../../entities/ccanvas.h"

Citrine::IGraphicsContext::IGraphicsContext(_PlatformContext* platform_context, unsigned int width, unsigned int height)
{
	igraphics = &glo_igraphics;
	context = igraphics->CreateContext(platform_context, width, height);
}

const void* Citrine::IGraphicsContext::CreateVBO(void* vertices, unsigned short vertices_amount, void* triangles, unsigned short triangles_amount)
{
	/*Create VBO*/
	BufferObject<MeshData>* vbo = new BufferObject<MeshData> ;
	vbo->buffer_object = igraphics->CreateVBO(context);
	/*Buffer VBO*/
	igraphics->BufferVBO(vbo->buffer_object, vertices, vertices_amount, triangles, triangles_amount);

	/*Add VBO to mesh data VBO list*/
	meshdatas.push_back(vbo);

	return vbo;
}

const void* Citrine::IGraphicsContext::CreateVBO()
{
	//Create VBO
	BufferObject<MeshData>* vbo = new BufferObject<MeshData> ;
	vbo->buffer_object = igraphics->CreateVBO(context);

	//Add VBO to mesh data VBO list*/
	meshdatas.push_back(vbo);

	return vbo;
}

void Citrine::IGraphicsContext::RebufferVBO(const void* vbo, void* vertices, unsigned short vertices_amount, void* triangles, unsigned short triangles_amount)
{
	BufferObject<MeshData>* _vbo = (BufferObject<MeshData>*) vbo;
	//Buffer VBO
	igraphics->BufferVBO(_vbo->buffer_object, vertices, vertices_amount, triangles, triangles_amount);
}

void Citrine::IGraphicsContext::DeleteVBO(const void* vbo)
{
	/*Search for mesh data in list*/
	for (std::list<BufferObject<MeshData>*>::iterator i = meshdatas.begin(); i != meshdatas.end(); i++)
	{
		/*Find VBO*/
		if (vbo == (*i)->buffer_object)
		{
			igraphics->DeleteVBO((*i)->buffer_object);
			delete *i;
			meshdatas.erase(i);
		}
	}
}

const void* Citrine::IGraphicsContext::AddTexture(SmartPointer<Texture> texture)
{
	/*Create TBO*/
	BufferObject<Texture>* tbo = new BufferObject<Texture> ;
	tbo->buffer_object = igraphics->CreateTBO(context);
	/*Buffer TBO*/
	igraphics->BufferTBO(tbo->buffer_object, texture->GetImage()->GetWidth(), texture->GetImage()->GetHeight(), texture->GetImage()->GetPixels());
	/*Add texture pointer to TBO*/
	tbo->parent = texture;

	/*Add TBO to mesh data TBO list*/
	textures.push_back(tbo);

	return tbo;
}

void Citrine::IGraphicsContext::RemoveTexture(SmartPointer<Texture> texture)
{
	/*Search for texture in list*/
	for (std::list<BufferObject<Texture>*>::iterator i = textures.begin(); i != textures.end(); i++)
	{
		/*If the texture is equal to the iterator, delete the TBO and remove it from the list*/
		if (*texture == *(*i)->parent)
		{
			igraphics->DeleteTBO((*i)->buffer_object);
			textures.erase(i);
		}
	}
}

const void* Citrine::IGraphicsContext::AddShader(Shader* shader)
{
	//Create shader object
	BufferObject<Shader>* shader_object = new BufferObject<Shader> ;
	shader_object->buffer_object = igraphics->CreateShader(context, shader->GetType());
	//Buffer shader
	igraphics->CompileShader(shader_object->buffer_object, shader->GetSource());
	//Add shader pointer to shader object
	shader_object->parent = shader;

	//Add shader object to shader list
	shaders.push_back(shader_object);

	return shader_object;
}

void Citrine::IGraphicsContext::RemoveShader(Shader* shader)
{
	//Search for shader in list
	for (std::list<BufferObject<Shader>*>::iterator i = shaders.begin(); i != shaders.end(); i++)
	{
		//If the shader is equal to the iterator, delete the shader and remove it from the list
		if (shader == *(*i)->parent)
		{
			igraphics->DeleteShader((*i)->buffer_object);
			shaders.erase(i);
		}
	}
}

const void* Citrine::IGraphicsContext::AddProgram(ShaderProgram* program)
{
	//Create program object
	BufferObject<ShaderProgram>* program_object = new BufferObject<ShaderProgram> ;
	program_object->buffer_object = igraphics->CreateProgram(context);
	//Add shaders
	for (std::list<SmartPointer<Shader> >::iterator i = program->GetShaders()->begin(); i != program->GetShaders()->end(); i++)
	{
		BufferObject<Shader>* shader = (BufferObject<Shader>*) (*i)->GetShaderObject();
		igraphics->ProgramUseShader(program_object->buffer_object, shader->buffer_object);
	}
	//Link program
	igraphics->LinkProgram(program_object->buffer_object);
	//Add program pointer to program object
	program_object->parent = program;

	//Add program object to shader list
	programs.push_back(program_object);

	return program_object;
}

void Citrine::IGraphicsContext::RemoveProgram(ShaderProgram* program)
{
	//Search for program in list
	for (std::list<BufferObject<ShaderProgram>*>::iterator i = programs.begin(); i != programs.end(); i++)
	{
		//If the program is equal to the iterator, delete the program and remove it from the list
		if (program == *(*i)->parent)
		{
			igraphics->DeleteProgram((*i)->buffer_object);
			programs.erase(i);
		}
	}
}

void Citrine::IGraphicsContext::RenderWorld(World* world)
{
	render_world = world;
}

void Citrine::IGraphicsContext::RenderCanvas(Canvas* canvas)
{
	render_canvases.push_back(canvas);
}

void Citrine::IGraphicsContext::FinishRender()
{
	/*Camera transformation*/
	Matrix4x4 camera_transformation = render_world->GetCurrentCamera()->GetTransformation()->GetMatrix()->GetInverse();

	/*Draw all lights in the world*/
	for (std::list<SmartPointer<Light> >::iterator i = render_world->GetLights()->begin(); i != render_world->GetLights()->end(); i++)
	{
		float* transformation_matrix = camera_transformation.GetArray();

		/*Get array of light components*/
		float* position = (*i)->GetPosition()->GetArray();
		float* ambient = (*i)->GetAmbient()->GetArray();
		float* diffuse = (*i)->GetDiffuse()->GetArray();
		float* specular = (*i)->GetSpecular()->GetArray();

		igraphics->DrawLight(context, position, ambient, diffuse, specular, transformation_matrix);

		/*Free arrays*/
		free(transformation_matrix);
		free(position);
		free(ambient);
		free(diffuse);
		free(specular);
	}

	/*Draw all meshes in the world*/
	for (std::list<SmartPointer<Mesh> >::iterator i = render_world->GetMeshes()->begin(); i != render_world->GetMeshes()->end(); i++)
	{
		/*Generate mesh transformation*/
		Matrix4x4 mesh_transformation = *(*i)->GetTransformation()->GetMatrix() * camera_transformation;

		/*If texture exists, bind it, else unbind texture*/
		/*if (*(*i)->GetMeshData()->GetTexture())
		{
			BufferObject<Texture>* texture = (BufferObject<Texture>*) (*i)->GetMeshData()->GetTexture()->GetBufferObject();
			igraphics->SetCurrentTBO(texture->buffer_object);
		}else
			igraphics->SetCurrentTBO(0);*/

		//If shader program exists, bind it, else unbind shader program
		if (*(*i)->GetMeshData()->GetShaderProgram())
		{
			BufferObject<ShaderProgram>* program = (BufferObject<ShaderProgram>*) (*i)->GetMeshData()->GetShaderProgram()->GetProgramObject();
			igraphics->SetCurrentProgram(context, program->buffer_object);
		}
		else
			igraphics->SetCurrentProgram(context, 0);

		/*Get buffer object and draw it*/
		BufferObject<MeshData>* buffer_object = (BufferObject<MeshData>*) (*i)->GetBufferObject();

		float* transformation_matrix = mesh_transformation.GetArray();
		igraphics->DrawVBO(buffer_object->buffer_object, transformation_matrix);
		free(transformation_matrix);
	}

	/*Draw all canvases*/
	for (std::list<Canvas*>::iterator i = render_canvases.begin(); i != render_canvases.end(); i++)
	{
		/*Draw all sprite2ds in canvas*/
		for (std::list<SmartPointer<Sprite2D> >::iterator sprite2d_i = (*i)->GetSprite2Ds()->begin(); sprite2d_i != (*i)->GetSprite2Ds()->end(); sprite2d_i++)
		{
			/*Get texture and position*/
			BufferObject<Texture>* texture = (BufferObject<Texture>*) (*sprite2d_i)->GetTexture()->GetBufferObject();
			Vector2* position = (*sprite2d_i)->GetPosition();
			/*Draw*/
			igraphics->DrawSprite2D(texture->buffer_object, position->x, position->y, (*sprite2d_i)->GetTexture()->GetImage()->GetWidth(), (*sprite2d_i)->GetTexture()->GetImage()->GetWidth());
		}
	}

	/*Clear render canvases*/
	render_canvases.clear();

	igraphics->FinishRender(context);
}
