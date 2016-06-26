/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include "ccamera.h"

using namespace Citrine;

Transform* Citrine::Camera::GetTransformation()
{
	return &transformation;
}
