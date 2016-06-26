/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CTRANSFORM_H
#define CTRANSFORM_H

#include "cmatrix.h"
#include "cvector.h"

namespace Citrine
{
	class Transform
	{
		public:
			Transform();
			Matrix4x4* GetMatrix();
			Matrix3x3 GetRotationMatrix();
			void SetRotationMatrix(Matrix3x3* matrix);
			void RotateOnXAxis(float xrot);
			void RotateOnYAxis(float yrot);
			void RotateOnZAxis(float zrot);
			void SetPositionToOther(Vector3* pos);
			void SetPosition(float xpos, float ypos, float zpos);
			void SetPositionX(float xpos);
			void SetPositionY(float ypos);
			void SetPositionZ(float zpos);
			Vector3 GetPosition();
			void MovePosition(float x, float y, float z);
			void MovePositionToOther(Vector3* pos);
			void MovePositionRelative(float x, float y, float z);
			void MovePositionRelativeToOther(Vector3* pos);
		private:
			Matrix4x4 tmatrix;
	};
}
#endif
