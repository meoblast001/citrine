/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <SDL/SDL.h>
#include "ctexture.h"

using namespace Citrine;

Citrine::Texture::Texture(IGraphicsContext* igraphicscontext)
{
	/*Store IGraphicsContext pointer*/
	this->igraphicscontext = igraphicscontext;
	buffer_object = 0;
}

Citrine::Texture::~Texture()
{
	/*If texture has been added to the graphics context, remove it*/
	if (buffer_object)
		igraphicscontext->RemoveTexture(this);
}

SmartPointer<Image> Citrine::Texture::GetImage()
{
	return image;
}

void Citrine::Texture::SetImage(SmartPointer<Image> image)
{
	this->image = image;

	/*Add texture to IGraphicsContext*/
	buffer_object = igraphicscontext->AddTexture(this);
}

const void* Citrine::Texture::GetBufferObject()
{
	return buffer_object;
}
