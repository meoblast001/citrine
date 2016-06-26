/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <stddef.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "cgraphics.h"
#include "../../cerror.h"
#include "../../entities/cmeshdata.h"

using namespace Citrine;
using namespace ErrorHandling;

IGraphics Citrine::glo_igraphics;

void OpenGLError(const char* details)
{
	//Get and report OpenGL errors if any exist
	switch (glGetError())
	{
		case GL_INVALID_ENUM:
			Error("OpenGL: %s: Invalid Enum", details);
			break;
		case GL_INVALID_VALUE:
			Error("OpenGL: %s: Invalid Value", details);
			break;
		case GL_INVALID_OPERATION:
			Error("OpenGL: %s: Invalid Operation", details);
			break;
		case GL_STACK_OVERFLOW:
			Error("OpenGL: %s: Stack Overflow", details);
			break;
		case GL_STACK_UNDERFLOW:
			Error("OpenGL: %s: Stack Underflow", details);
			break;
		case GL_OUT_OF_MEMORY:
			Error("OpenGL: %s Out of Memory", details);
			break;
	}
}

Citrine::IGraphics::~IGraphics()
{
	/*Delete all contexts*/
	for (std::list<ContextItem*>::iterator i = contexts.begin(); i != contexts.end(); i++)
		delete *i;
}

const void* Citrine::IGraphics::CreateContext(PlatformContext platform_context, unsigned int width, unsigned int height)
{
	/*Create new internal context*/
	ContextItem* context_item = new ContextItem;

	/*Store platform context and set it to current*/
	context_item->platform_context = platform_context;
	SetCurrentContext(platform_context);

	/*Copy width and height values*/
	context_item->width = width;
	context_item->height = height;

	/*Construct perspective ratio*/
	GLfloat sratio;
	sratio = (GLfloat) width / (GLfloat) height;

	/*Set viewport size*/
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, sratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*Set default OpenGL properties*/
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(100.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LIGHTING);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	/*Set current light*/
	context_item->currentlight = 0;
	/*Add new context to list*/
	contexts.push_back(context_item);

	/*Initialize GLEW*/
	glewInit();

	return context_item;
}

void Citrine::IGraphics::DeleteContext(const void* context)
{
	ContextItem* typecast_context = (ContextItem*) context;
	SetCurrentContext(typecast_context->platform_context);

	/*Search for context in list*/
	for (std::list<ContextItem*>::iterator i = contexts.begin(); i != contexts.end(); i++)
	{
		/*If the context is equal to the iterator, delete it and remove it from the list*/
		if (typecast_context == *i)
		{
			delete *i;
			contexts.erase(i);
			return;
		}
	}
}

const void* Citrine::IGraphics::CreateVBO(const void* context)
{
	ContextItem* typecast_context = (ContextItem*) context;
	SetCurrentContext(typecast_context->platform_context);

	/*Create VBO instance*/
	VertexBufferObject* vbo = new VertexBufferObject;

	/*Check if the OpenGL implementation supports VBOs*/
	if (glewIsSupported("GL_ARB_vertex_buffer_object"))
	{
		glGenBuffersARB(2, vbo->vbos);
		OpenGLError("CreateVBO/glGenBuffersARB");
	}

	/*Add reference of context to VBO instance*/
	vbo->parent = typecast_context;

	/*Add new VBO to list*/
	typecast_context->vbos.push_back(vbo);

	return vbo;
}

void Citrine::IGraphics::BufferVBO(const void* vbo, const void* vertex_data, unsigned int vertex_dsize, const void* index_data, unsigned int index_dsize)
{
	VertexBufferObject* typecast_vbo = (VertexBufferObject*) vbo;
	SetCurrentContext(typecast_vbo->parent->platform_context);

	/*Store size of index buffer*/
	typecast_vbo->index_size = index_dsize;

	/*Buffer vertex buffer*/
	glBindBufferARB(GL_ARRAY_BUFFER, typecast_vbo->vbos[0]);
	OpenGLError("BufferVBO/glBindBufferARB/vertex");
	glBufferDataARB(GL_ARRAY_BUFFER, vertex_dsize * sizeof(Vertex), vertex_data, GL_STATIC_DRAW);
	OpenGLError("BufferVBO/glBufferDataARB/vertex");

	/*Buffer index buffer*/
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, typecast_vbo->vbos[1]);
	OpenGLError("BufferVBO/glBindBufferARB/index");
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, index_dsize * sizeof(Triangle), index_data, GL_STATIC_DRAW);
	OpenGLError("BufferVBO/glBufferDataARB/index");

	/*Unbind current buffers*/
	glBindBufferARB(GL_ARRAY_BUFFER, 0);
	OpenGLError("BufferVBO/glBindBufferARB/unbind/GL_ARRAY_BUFFER");
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
	OpenGLError("BufferVBO/glBindBufferARB/unbind/GL_ELEMENT_ARRAY_BUFFER");
}

void Citrine::IGraphics::DeleteVBO(const void* vbo)
{
	VertexBufferObject* typecast_vbo = (VertexBufferObject*) vbo;
	SetCurrentContext(typecast_vbo->parent->platform_context);

	/*Search for VBO in list*/
	for (std::list<VertexBufferObject*>::iterator i = typecast_vbo->parent->vbos.begin(); i != typecast_vbo->parent->vbos.end(); i++)
	{
		/*If the VBO is equal to the iterator, delete the buffer and remove it from the list*/
		if (typecast_vbo == *i)
		{
			glDeleteBuffersARB(2, typecast_vbo->vbos);
			OpenGLError("DeleteVBO/glDeleteBuffersARB");
			delete typecast_vbo;
			typecast_vbo->parent->vbos.erase(i);
			return;
		}
	}
}

void Citrine::IGraphics::DrawVBO(const void* vbo, float* transformation)
{
	VertexBufferObject* typecast_vbo = (VertexBufferObject*) vbo;
	SetCurrentContext(typecast_vbo->parent->platform_context);

	/*Transform*/
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(transformation);
	OpenGLError("DrawVBO/glLoadMatrixf");

	/*Vertices*/
	glBindBufferARB(GL_ARRAY_BUFFER, typecast_vbo->vbos[0]);
	OpenGLError("DrawVBO/glBindBufferARB/vertex");
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (GLvoid*) offsetof(Vertex, position));
	OpenGLError("DrawVBO/glVertexPointer");
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid*) offsetof(Vertex, texcoord));
	OpenGLError("DrawVBO/glTexCoordPointer");
	/*Triangles*/
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, typecast_vbo->vbos[1]);
	OpenGLError("DrawVBO/glBindBufferARB/index");
	glDrawElements(GL_TRIANGLES, typecast_vbo->index_size * 3, GL_UNSIGNED_INT, 0);
	OpenGLError("DrawVBO/glDrawElements");

	/*Unbind current buffers*/
	glBindBufferARB(GL_ARRAY_BUFFER, 0);
	OpenGLError("DrawVBO/glBindBufferARB/unbind/GL_ARRAY_BUFFER");
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
	OpenGLError("DrawVBO/glBindBufferARB/unbind/GL_ELEMENT_ARRAY_BUFFER");
}

const void* Citrine::IGraphics::CreateTBO(const void* context)
{
	ContextItem* typecast_context = (ContextItem*) context;
	SetCurrentContext(typecast_context->platform_context);

	/*Create TBO instance*/
	GenericBufferObject* tbo = new GenericBufferObject;

	/*Generate TBO*/
	glGenTextures(1, &tbo->handle);
	OpenGLError("CreateTBO/glGenTextures");

	/*Add reference of context to TBO instance*/
	tbo->parent = typecast_context;

	/*Add new TBO to list*/
	typecast_context->tbos.push_back(tbo);

	return tbo;
}

void Citrine::IGraphics::BufferTBO(const void* tbo, unsigned int width, unsigned int height, const void* pixels)
{
	GenericBufferObject* typecast_tbo = (GenericBufferObject*) tbo;
	SetCurrentContext(typecast_tbo->parent->platform_context);

	glBindTexture(GL_TEXTURE_2D, typecast_tbo->handle);
	OpenGLError("BufferTBO/glBindTexture");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	OpenGLError("BufferTBO/glTexImage2D");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	OpenGLError("BufferTBO/glTexParameteri/GL_TEXTURE_MIN_FILTER");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	OpenGLError("BufferTBO/glTexParameteri/GL_TEXTURE_MAG_FILTER");
}

void Citrine::IGraphics::DeleteTBO(const void* tbo)
{
	GenericBufferObject* typecast_tbo = (GenericBufferObject*) tbo;
	SetCurrentContext(typecast_tbo->parent->platform_context);

	/*Search for TBO in list*/
	for (std::list<GenericBufferObject*>::iterator i = typecast_tbo->parent->tbos.begin(); i != typecast_tbo->parent->tbos.end(); i++)
	{
		/*If the TBO is equal to the iterator, delete the buffer and remove it from the list*/
		if (typecast_tbo == *i)
		{
			glDeleteTextures(1, &typecast_tbo->handle);
			OpenGLError("DeleteTBO/glDeleteTextures");
			delete *i;
			typecast_tbo->parent->tbos.erase(i);
		}
	}
}

void Citrine::IGraphics::SetCurrentTBO(const void* tbo)
{
	GenericBufferObject* typecast_tbo = (GenericBufferObject*) tbo;
	SetCurrentContext(typecast_tbo->parent->platform_context);

	/*If texture is non-zero, bind it, else unbind textures*/
	if (typecast_tbo)
	{
		glBindTexture(GL_TEXTURE_2D, typecast_tbo->handle);
		OpenGLError("SetCurrentTBO/glBindTexture/bind");
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		OpenGLError("SetCurrentTBO/glBindTexture/unbind");
	}
}

static GLenum GetLight(unsigned char light)
{
	switch (light)
	{
		case 0:
			return GL_LIGHT0;
		case 1:
			return GL_LIGHT1;
		case 2:
			return GL_LIGHT2;
		case 3:
			return GL_LIGHT3;
		case 4:
			return GL_LIGHT4;
		case 5:
			return GL_LIGHT5;
		case 6:
			return GL_LIGHT6;
		case 7:
			return GL_LIGHT7;
	}
}

void Citrine::IGraphics::DrawLight(const void* context, float* position, float* ambient, float* diffuse, float* specular, float* transformation)
{
	ContextItem* typecast_context = (ContextItem*) context;
	SetCurrentContext(typecast_context->platform_context);

	GLenum light = GetLight(typecast_context->currentlight);
	typecast_context->currentlight = typecast_context->currentlight == 8 ? 0 : typecast_context->currentlight + 1;

	/*Transform*/
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(transformation);
	OpenGLError("DrawLight/glLoadMatrixf");

	glLightfv(light, GL_POSITION, position);
	OpenGLError("DrawLight/glLightfv/GL_POSITION");
	glLightfv(light, GL_AMBIENT, ambient);
	OpenGLError("DrawLight/glLightfv/GL_AMBIENT");
	glLightfv(light, GL_DIFFUSE, diffuse);
	OpenGLError("DrawLight/glLightfv/GL_DIFFUSE");
	glLightfv(light, GL_SPECULAR, specular);
	OpenGLError("DrawLight/glLightfv/GL_SPECULAR");
	glEnable(light);
	OpenGLError("DrawLight/glEnable/light");
}

void Citrine::IGraphics::DrawSprite2D(const void* tbo, float x, float y, float width, float height)
{
	GenericBufferObject* typecast_tbo = (GenericBufferObject*) tbo;
	SetCurrentContext(typecast_tbo->parent->platform_context);

	SetCurrentTBO(typecast_tbo);

	/*Disable depth testing*/
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	/*Operations will only be performed in 2D temporarily*/
	glPushMatrix();
	/*Preapre projection matrix for 2D drawing*/
	glLoadIdentity();
	gluOrtho2D(0, typecast_tbo->parent->width, 0, typecast_tbo->parent->height);
	glScalef(1, -1, 1);
	glTranslatef(0, -typecast_tbo->parent->height, 0);
	/*Return to model view matrix*/
	glMatrixMode(GL_MODELVIEW);

	/*Generate poly data*/
	float vertices[] = {/*Vertex 0*/0.0f, 0.0f, /*Vertex 1*/0.0f, 1.0f, /*Vertex 2*/1.0f, 1.0f, /*Vertex 3*/1.0f, 0.0f};
	float texcoords[] = {/*Texcoord 0*/0.0f, 0.0f, /*Texcoord 1*/0.0f, 1.0f, /*Texcoord 2*/1.0f, 1.0f, /*Texcoord 3*/1.0f, 0.0f};
	float indices[] = {0, 1, 2, 3};

	vertices[3] = height;
	vertices[4] = width;
	vertices[5] = height;
	vertices[6] = width;

	/*Draw poly*/
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	OpenGLError("DrawSprite2D/glVertexPointer");
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	OpenGLError("DrawSprite2D/glTexCoordPointer");
	glDrawElements(GL_QUADS, 4, GL_FLOAT, indices);
	OpenGLError("DrawSprite2D/glDrawElements");

	/*Reset projection matrix*/
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	/*Return to modelview matrix*/
	glMatrixMode(GL_MODELVIEW);
	/*Enable depth testing*/
	glEnable(GL_DEPTH_TEST);
}

const void* Citrine::IGraphics::CreateShader(const void* context, ShaderType shader_type)
{
	ContextItem* typecast_context = (ContextItem*) context;
	SetCurrentContext(typecast_context->platform_context);

	//Create shader object
	GenericBufferObject* shader = new GenericBufferObject;

	//Convert shader type to OpenGL shader types
	GLenum gl_shader_type;
	if (shader_type == VERTEX_SHADER)
		gl_shader_type = GL_VERTEX_SHADER;
	else if (shader_type == FRAGMENT_SHADER)
		gl_shader_type = GL_FRAGMENT_SHADER;

	if (glewIsSupported("GL_ARB_shader_objects GL_ARB_vertex_shader GL_ARB_fragment_shader"))
	{
		shader->handle = glCreateShaderObjectARB(gl_shader_type);
		OpenGLError("CreateShader/glCreateShaderObjectARB");
	}
	else
		Error("Shaders not supported");

	//Add reference of context to shader object
	shader->parent = typecast_context;

	//Add new shader object to list
	typecast_context->shaders.push_back(shader);

	return shader;
}

void Citrine::IGraphics::CompileShader(const void* shader, const char* shader_string)
{
	GenericBufferObject* typecast_shader = (GenericBufferObject*) shader;
	SetCurrentContext(typecast_shader->parent->platform_context);

	//Provide shader with shader source string
	glShaderSourceARB(typecast_shader->handle, 1, &shader_string, 0);
	OpenGLError("CompileShader/glShaderSourceARB");
	//Compile shader
	glCompileShaderARB(typecast_shader->handle);
	OpenGLError("CompileShader/glCompileShaderARB");
}

void Citrine::IGraphics::DeleteShader(const void* shader)
{
	GenericBufferObject* typecast_shader = (GenericBufferObject*) shader;
	SetCurrentContext(typecast_shader->parent->platform_context);

	//Search for shader in list
	for (std::list<GenericBufferObject*>::iterator i = typecast_shader->parent->shaders.begin(); i != typecast_shader->parent->shaders.end(); i++)
	{
		//If the shader is equal to the iterator, delete the shader and remove it from the list
		if (typecast_shader = *i)
		{
			glDeleteShader(typecast_shader->handle);
			OpenGLError("DeleteShader/glDeleteShader");
			delete typecast_shader;
			typecast_shader->parent->shaders.erase(i);
			return;
		}
	}
}

const void* Citrine::IGraphics::CreateProgram(const void* context)
{
	ContextItem* typecast_context = (ContextItem*) context;
	SetCurrentContext(typecast_context->platform_context);

	//Create program object
	GenericBufferObject* program = new GenericBufferObject;

	program->handle = glCreateProgramObjectARB();
	OpenGLError("CreateProgram/glCreateProgramObjectARB");

	//Add reference of context to program object
	program->parent = typecast_context;

	//Add new program object to list
	typecast_context->programs.push_back(program);

	return program;
}

void Citrine::IGraphics::ProgramUseShader(const void* program, const void* shader)
{
	GenericBufferObject* typecast_program = (GenericBufferObject*) program;
	GenericBufferObject* typecast_shader = (GenericBufferObject*) shader;
	SetCurrentContext(typecast_program->parent->platform_context);

	//Attach shader object to program object
	glAttachObjectARB(typecast_program->handle, typecast_shader->handle);
	OpenGLError("ProgramUseShader/glAttachObjectARB");
}

void Citrine::IGraphics::LinkProgram(const void* program)
{
	GenericBufferObject* typecast_program = (GenericBufferObject*) program;
	SetCurrentContext(typecast_program->parent->platform_context);

	//Link program
	glLinkProgramARB(typecast_program->handle);
	OpenGLError("LinkProgram/glLinkProgramARB");
}

void Citrine::IGraphics::DeleteProgram(const void* program)
{
	GenericBufferObject* typecast_program = (GenericBufferObject*) program;
	SetCurrentContext(typecast_program->parent->platform_context);

	//Search for program in list
	for (std::list<GenericBufferObject*>::iterator i = typecast_program->parent->programs.begin(); i != typecast_program->parent->programs.end(); i++)
	{
		//If the program is equal to the iterator, delete the program and remove it from the list
		if (typecast_program = *i)
		{
			glDeleteProgram(typecast_program->handle);
			OpenGLError("DeleteProgram/glDeleteProgram");
			delete typecast_program;
			typecast_program->parent->programs.erase(i);
			return;
		}
	}
}

void Citrine::IGraphics::SetCurrentProgram(const void* context, const void* program)
{
	ContextItem* typecast_context = (ContextItem*) context;
	GenericBufferObject* typecast_program = (GenericBufferObject*) program;
	SetCurrentContext(typecast_context->platform_context);

	if (typecast_program)
	{
		glUseProgram(typecast_program->handle);
		OpenGLError("SetCurrentProgram/glUseProgram/bind");
	}
	else
	{
		glUseProgram(0);
		OpenGLError("SetCurrentProgram/glUserProgram/unbind");
	}
}

void Citrine::IGraphics::FinishRender(const void* context)
{
	ContextItem* typecast_context = (ContextItem*) context;

	SetCurrentContext(typecast_context->platform_context);
	/*Swap buffers*/
	ContextSwapBuffers(typecast_context->platform_context);
	/*Clear screen*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	OpenGLError("FinishRender/glClear");

	/*Reset current light*/
	typecast_context->currentlight = 0;
}
