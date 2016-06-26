/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CMESHDATA_H
#define CMESHDATA_H

#include <utility>
#include "cpoly.h"
#include "../images/ctexture.h"
#include "../shaders/cshaderprogram.h"
#include "cvertex.h"
#include "../csmartpointer.h"
#include "../contexts/graphics/cgraphicscontext.h"
#include "carmature.h"

namespace Citrine
{
	class IGraphicsContext;

	class MeshData
	{
		public:
			MeshData();
			virtual void* GetAnimation(const char* name) { }
			virtual unsigned short GetFramesAmount(void* animation) { }
			virtual std::pair<Vertex*, unsigned short> GetVertices(void* animation, unsigned short frame) { }
			virtual std::pair<Triangle*, unsigned short> GetTriangles(void* animation, unsigned short frame) { }
			virtual std::pair<Armature*, unsigned short> GetArmatures(void* animation, unsigned short frame) { }
			virtual const void* GetBufferObject(void* animation, unsigned short frame) { }
			SmartPointer<Texture> GetTexture();
			void SetTexture(SmartPointer<Texture> texture);
			SmartPointer<ShaderProgram> GetShaderProgram();
			void SetShaderProgram(SmartPointer<ShaderProgram> shader_program);
		protected:
			IGraphicsContext* igraphicscontext;
		private:
			SmartPointer<Texture> texture;
			SmartPointer<ShaderProgram> shader_program;
	};

	class VertexMeshData : public MeshData
	{
		public:
			struct Frame
			{
				Vertex* vertices;
				unsigned short vertices_amount;
				Triangle* triangles;
				unsigned short triangles_amount;
				const void* buffer_object;
			};

			struct Animation
			{
				char* name;
				unsigned char frame_rate;
				Frame* frames;
				unsigned short frames_amount;
			};

			VertexMeshData(IGraphicsContext* igraphicscontext, Frame* base_frame, Animation* animations, unsigned short animations_amount);
			~VertexMeshData();
			void* GetAnimation(const char* name);
			unsigned short GetFramesAmount(void* animation);
			std::pair<Vertex*, unsigned short> GetVertices(void* animation, unsigned short frame);
			std::pair<Triangle*, unsigned short> GetTriangles(void* animation, unsigned short frame);
			std::pair<Armature*, unsigned short> GetArmatures(void* animation, unsigned short frame);
			const void* GetBufferObject(void* animation, unsigned short frame);
			void Enable();
			void Disable();
		private:
			Frame* base_frame;
			Animation* animations;
			unsigned short animations_amount;
	};

	class ArmatureMeshData : public MeshData
	{
		public:
			struct Frame
			{
				Armature* armatures;
				unsigned char armatures_amount;
			};

			struct Animation
			{
				char* name;
				unsigned char frame_rate;
				Frame* frames;
				unsigned short frames_amount;
			};

			ArmatureMeshData(IGraphicsContext* igraphicscontext, Frame* base_frame, Animation* animations, unsigned short animations_amount, Vertex* vertices, unsigned short vertices_amount, Triangle* triangles, unsigned short triangles_amount);
			~ArmatureMeshData();
			void* GetAnimation(const char* name);
			unsigned short GetFramesAmount(void* animation);
			std::pair<Vertex*, unsigned short> GetVertices(void* animation, unsigned short frame);
			std::pair<Triangle*, unsigned short> GetTriangles(void* animation, unsigned short frame);
			std::pair<Armature*, unsigned short> GetArmatures(void* animation, unsigned short frame);
			const void* GetBufferObject(void* animation, unsigned short frame);
			void Enable();
			void Disable();
		private:
			Vertex* vertices;
			unsigned short vertices_amount;
			Triangle* triangles;
			unsigned short triangles_amount;
			const void* buffer_object;

			Frame* base_frame;
			Animation* animations;
			unsigned short animations_amount;
	};
}
#endif
