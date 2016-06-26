/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cshader.h"
#include "../cerror.h"

using namespace Citrine;
using namespace ErrorHandling;

Citrine::Shader::Shader(IGraphicsContext* igraphicscontext, const char* filename)
{
	//Store IGraphicsContext pointer
	this->igraphicscontext = igraphicscontext;
	type = (ShaderType) 0;

	FILE* file = fopen(filename, "r");
	//If file handle is null, an error has occurred
	if (!file)
		Error("Shader file \"%s\" not found", filename);

	//Get file size
	fseek(file, 0, SEEK_END);
	unsigned int file_size = ftell(file);
	rewind(file);

	//Allocate source buffer and read file contents to it
	shader_source = (char*) malloc(file_size + 1);
	fread(shader_source, 1, file_size, file);
	//Null termination
	shader_source[file_size] = 0;
	fclose(file);

	//Get shader type
	if (!strncmp(filename + strlen(filename) - 3, ".vs", 3))
		type = VERTEX_SHADER;
	else if (!strncmp(filename + strlen(filename) - 3, ".fs", 3))
		type = FRAGMENT_SHADER;

	//Create shader object
	shader_object = igraphicscontext->AddShader(this);
}

Citrine::Shader::~Shader()
{
	//Remove shader from graphics context
	igraphicscontext->RemoveShader(this);

	//Free source
	free(shader_source);
}

const char* Citrine::Shader::GetSource()
{
	return shader_source;
}

ShaderType Citrine::Shader::GetType()
{
	return type;
}

const void* Citrine::Shader::GetShaderObject()
{
	return shader_object;
}
