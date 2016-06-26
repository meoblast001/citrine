/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include "csprite3d.h"

using namespace Citrine;

SmartPointer<Texture> Citrine::Sprite3D::GetTexture()
{
	return texture;
}

void Citrine::Sprite3D::SetTexture(SmartPointer<Texture> texture)
{
	this->texture = texture;
}

Transform* Citrine::Sprite3D::GetTransform()
{
	return &transformation;
}
