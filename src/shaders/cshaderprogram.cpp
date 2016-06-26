/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include "cshaderprogram.h"
#include "cshader.h"
#include "../contexts/graphics/cgraphicscontext.h"
#include "../csmartpointer.h"

using namespace Citrine;

Citrine::ShaderProgram::ShaderProgram(IGraphicsContext* igraphicscontext)
{
	this->igraphicscontext = igraphicscontext;
	program_object = 0;
}

Citrine::ShaderProgram::~ShaderProgram()
{
	//If the program object is valid, remove it from the IGraphicsContext
	if (program_object)
		igraphicscontext->RemoveProgram(this);
}

std::list<SmartPointer<Shader> >* Citrine::ShaderProgram::GetShaders()
{
	return &shaders;
}

void Citrine::ShaderProgram::AddShader(SmartPointer<Shader> shader)
{
	shaders.push_back(shader);
}

const void* Citrine::ShaderProgram::GetProgramObject()
{
	return program_object;
}

void Citrine::ShaderProgram::Enable()
{
	program_object = igraphicscontext->AddProgram(this);
}
