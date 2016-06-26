/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include "clight.h"

using namespace Citrine;

Citrine::Light::Light()
{
	position = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	ambient = Vector4(0.2f, 0.2f, 0.2f, 1.0f);
	diffuse = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
	specular = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}

Vector4* Citrine::Light::GetPosition()
{
	return &position;
}
Vector4* Citrine::Light::GetAmbient()
{
	return &ambient;
}
Vector4* Citrine::Light::GetDiffuse()
{
	return &diffuse;
}
Vector4* Citrine::Light::GetSpecular()
{
	return &specular;
}
