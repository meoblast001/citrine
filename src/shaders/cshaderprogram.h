/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CSHADERPROGRAM_H
#define CSHADERPROGRAM_H

#include <list>
#include "cshader.h"
#include "../contexts/graphics/cgraphicscontext.h"

namespace Citrine
{
	class ShaderProgram
	{
		public:
			ShaderProgram(IGraphicsContext* igraphicscontext);
			~ShaderProgram();
			std::list<SmartPointer<Shader> >* GetShaders();
			void AddShader(SmartPointer<Shader> shader);
			const void* GetProgramObject();
			void Enable();
		private:
			std::list<SmartPointer<Shader> > shaders;
			IGraphicsContext* igraphicscontext;
			const void* program_object;
	};
}
#endif
