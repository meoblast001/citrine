/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CGRAPHICSCONTEXT_H
#define CGRAPHICSCONTEXT_H

#include <list>
#include "../../csmartpointer.h"

namespace Citrine
{
	class IGraphics;
	class MeshData;
	class Texture;
	class Shader;
	class ShaderProgram;
	class World;
	class Canvas;
	struct _PlatformContext;

	class IGraphicsContext
	{
		public:
			IGraphicsContext(_PlatformContext* platform_context, unsigned int width, unsigned int height);
			const void* CreateVBO(void* vertices, unsigned short vertices_amount, void* triangles, unsigned short triangles_amount);
			const void* CreateVBO();
			void RebufferVBO(const void* vbo, void* vertices, unsigned short vertices_amount, void* triangles, unsigned short triangles_amount);
			void DeleteVBO(const void* vbo);
			const void* AddTexture(SmartPointer<Texture> texture);
			void RemoveTexture(SmartPointer<Texture> texture);
			const void* AddShader(Shader* shader);
			void RemoveShader(Shader* shader);
			const void* AddProgram(ShaderProgram* program);
			void RemoveProgram(ShaderProgram* program);
			void RenderWorld(World* world);
			void RenderCanvas(Canvas* canvas);
			void FinishRender();
		private:
			template<typename T>
			struct BufferObject
			{
				const void* buffer_object;
				SmartPointer<T> parent;
			};

			IGraphics* igraphics;
			const void* context;
			std::list<BufferObject<MeshData>*> meshdatas;
			std::list<BufferObject<Texture>*> textures;
			std::list<BufferObject<Shader>*> shaders;
			std::list<BufferObject<ShaderProgram>*> programs;
			/*World and canvases to draw*/
			World* render_world;
			std::list<Canvas*> render_canvases;
	};
}
#endif
