/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CSHADER_H
#define CSHADER_H

#include "../contexts/graphics/cgraphicscontext.h"

namespace Citrine
{
	enum ShaderType
	{
		VERTEX_SHADER,
		FRAGMENT_SHADER
	};

	class Shader
	{
		public:
			Shader(IGraphicsContext* igraphicscontext, const char* filename);
			~Shader();
			const char* GetSource();
			ShaderType GetType();
			const void* GetShaderObject();
		private:
			char* shader_source;
			ShaderType type;
			IGraphicsContext* igraphicscontext;
			const void* shader_object;
	};
}
#endif
