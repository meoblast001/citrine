/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CQUATERNION_H
#define CQUATERNION_H

#include "cvector.h"
#include "cmatrix.h"

namespace Citrine
{
	class Quaternion
	{
		public:
			Vector4 q;

			Quaternion() { }
			Quaternion(float x, float y, float z, float w);
			Matrix3x3 ConvertToMatrix();
	};
}
#endif
