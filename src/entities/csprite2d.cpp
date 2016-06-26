/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include "csprite2d.h"

using namespace Citrine;

SmartPointer<Texture> Citrine::Sprite2D::GetTexture()
{
	return texture;
}

void Citrine::Sprite2D::SetTexture(SmartPointer<Texture> texture)
{
	this->texture = texture;
}

Vector2* Citrine::Sprite2D::GetPosition()
{
	return &position;
}
