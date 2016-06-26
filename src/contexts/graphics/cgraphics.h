/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CGRAPHICS_H
#define CGRAPHICS_H

#include <list>
#include "../crossplatform/ccrossplatform.h"
#include "../../shaders/cshader.h"

struct SDL_Surface;

namespace Citrine
{
	class IGraphics
	{
		public:
			~IGraphics();
			const void* CreateContext(PlatformContext platform_context, unsigned int width, unsigned int height);
			void DeleteContext(const void* context);
			const void* CreateVBO(const void* context);
			void BufferVBO(const void* vbo, const void* vertex_data, unsigned int vertex_dsize, const void* index_data, unsigned int index_dsize);
			void DeleteVBO(const void* vbo);
			void DrawVBO(const void* vbo, float* transformation);
			const void* CreateTBO(const void* context);
			void BufferTBO(const void* tbo, unsigned int width, unsigned int height, const void* pixels);
			void DeleteTBO(const void* tbo);
			void SetCurrentTBO(const void* tbo);
			void DrawLight(const void* context, float* position, float* ambient, float* diffuse, float* specular, float* transformation);
			void DrawSprite2D(const void* tbo, float x, float y, float width, float height);
			const void* CreateShader(const void* context, ShaderType shader_type);
			void CompileShader(const void* shader, const char* shader_string);
			void DeleteShader(const void* shader);
			const void* CreateProgram(const void* context);
			void ProgramUseShader(const void* program, const void* shader);
			void LinkProgram(const void* program);
			void DeleteProgram(const void* program);
			void SetCurrentProgram(const void* context, const void* program);
			void FinishRender(const void* context);
		private:
			struct ContextItem;

			struct VertexBufferObject
			{
				unsigned int vbos[2];
				ContextItem* parent;
				unsigned int index_size;
			};

			struct GenericBufferObject
			{
				unsigned int handle;
				ContextItem* parent;
			};

			struct ContextItem
			{
				unsigned int context;
				PlatformContext platform_context;
				unsigned int width;
				unsigned int height;
				std::list<VertexBufferObject*> vbos;
				std::list<GenericBufferObject*> tbos;
				std::list<GenericBufferObject*> shaders;
				std::list<GenericBufferObject*> programs;
				unsigned char currentlight;
			};

			std::list<ContextItem*> contexts;
	};

	extern IGraphics glo_igraphics;
}
#endif
