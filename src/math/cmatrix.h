/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CMATRIX_H
#define CMATRIX_H

#include "cvector.h"

namespace Citrine
{
	class Matrix2x2
	{
		public:
			Vector2 x, y;

			Matrix2x2() { }
			Matrix2x2(const Vector2& x, const Vector2& y);
			Matrix2x2 GetConcatenate(const Matrix2x2& other);
			void Concatenate(const Matrix2x2& other);
			Matrix2x2 GetConcatenate(const float other);
			void Concatenate(const float other);
			float* GetArray();
			void Set(const Matrix2x2& other);

			Matrix2x2 operator*(const Matrix2x2& other)
			{
				return GetConcatenate(other);
			}

			void operator*=(const Matrix2x2& other)
			{
				Concatenate(other);
			}

			void operator=(const Matrix2x2& other)
			{
				Set(other);
			}
	};

	class Matrix3x3
	{
		public:
			Vector3 x, y, z;

			Matrix3x3() { }
			Matrix3x3(const Vector3& x, const Vector3& y, const Vector3& z);
			Matrix3x3 GetConcatenate(const Matrix3x3& other);
			void Concatenate(const Matrix3x3& other);
			Matrix3x3 GetConcatenate(const float other);
			void Concatenate(const float other);
			Vector3 GetConcatenate(const Vector3& other);
			float* GetArray();
			void Set(const Matrix3x3& other);

			Matrix3x3 operator*(Matrix3x3 other)
			{
				return GetConcatenate(other);
			}

			void operator*=(const Matrix3x3& other)
			{
				Concatenate(other);
			}

			Vector3 operator*(const Vector3& other)
			{
				return GetConcatenate(other);
			}

			void operator=(const Matrix3x3& other)
			{
				Set(other);
			}
	};

	class Matrix4x4
	{
		public:
			Vector4 x, y, z, w;

			Matrix4x4() { }
			Matrix4x4(const Vector4& x, const Vector4& y, const Vector4& z, const Vector4& w);
			Matrix4x4 GetConcatenate(const Matrix4x4& other);
			void Concatenate(const Matrix4x4& other);
			Matrix4x4 GetConcatenate(const float other);
			void Concatenate(const float other);
			float* GetArray();
			void Set(const Matrix4x4& other);
			float GetDeterminant();
			Matrix4x4 GetInverse();

			Matrix4x4 operator*(const Matrix4x4& other)
			{
				return GetConcatenate(other);
			}

			void operator*=(const Matrix4x4& other)
			{
				Concatenate(other);
			}

			void operator =(const Matrix4x4& other)
			{
				Set(other);
			}
	};
}
#endif
