/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CVECTOR_H
#define CVECTOR_H

namespace Citrine
{
	class Vector2
	{
		public:
			float x, y;

			Vector2() { }
			Vector2(const float x, const float y);
			Vector2 GetAdd(const Vector2& other);
			void Add(const Vector2& other);
			Vector2 GetConcatenate(const float other);
			void Concatenate(const float other);
			void Set(const Vector2& other);
			float* GetArray();

			Vector2 operator+(const Vector2& other)
			{
				return GetAdd(other);
			}

			void operator+=(const Vector2& other)
			{
				Add(other);
			}

			Vector2 operator*(const float other)
			{
				return GetConcatenate(other);
			}

			void operator*=(const float other)
			{
				Concatenate(other);
			}
	};

	class Vector3
	{
		public:
			float x, y, z;

			Vector3() { }
			Vector3(const float x, const float y, const float z);
			Vector3 GetAdd(const Vector3& other);
			void Add(const Vector3& other);
			Vector3 GetConcatenate(const float other);
			void Concatenate(const float other);
			void Set(const Vector3& other);
			float* GetArray();

			Vector3 operator+(const Vector3& other)
			{
				return GetAdd(other);
			}

			void operator+=(const Vector3& other)
			{
				Add(other);
			}

			Vector3 operator*(const float other)
			{
				return GetConcatenate(other);
			}

			void operator*=(const float other)
			{
				Concatenate(other);
			}

			void operator=(const Vector3& other)
			{
				Set(other);
			}
	};

	class Vector4
	{
		public:
			float x, y, z, w;

			Vector4() { }
			Vector4(const float x, const float y, const float z, const float w);
			Vector4 GetAdd(const Vector4& other);
			void Add(const Vector4& other);
			Vector4 GetConcatenate(const float other);
			void Concatenate(const float other);
			void Set(const Vector4& other);
			float* GetArray();

			Vector4 operator+(const Vector4& other)
			{
				return GetAdd(other);
			}

			void operator+=(const Vector4& other)
			{
				Add(other);
			}

			Vector4 operator*(const float& other)
			{
				return GetConcatenate(other);
			}

			void operator*=(const float& other)
			{
				Concatenate(other);
			}

			void operator=(const Vector4& other)
			{
				Set(other);
			}
	};
}
#endif
