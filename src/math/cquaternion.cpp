/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include "cquaternion.h"

using namespace Citrine;

Citrine::Quaternion::Quaternion(float x, float y, float z, float w)
{
	q.x = x;
	q.y = y;
	q.z = z;
	q.w = w;
}

Matrix3x3 Citrine::Quaternion::ConvertToMatrix()
{
	Matrix3x3 result;

	result.x.x = 1.0f - 2.0f * (q.z * q.z + q.w * q.w);
	result.x.y = 2.0f * (q.y * q.z + q.w * q.x);
	result.x.z = 2.0f * (q.y * q.w - q.z * q.x);

	result.y.x = 2.0f * (q.y * q.z - q.w * q.x);
	result.y.y = 1.0f - 2.0f * (q.y * q.y + q.w * q.w);
	result.y.z = 2.0f * (q.z * q.w + q.y * q.x);

	result.z.x = 2.0f * (q.y * q.w + q.z * q.x);
	result.z.y = 2.0f * (q.z * q.w - q.y * q.x);
	result.z.z = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);

	return result;
}
