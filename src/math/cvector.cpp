/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include "cvector.h"
#include <stdlib.h>

using namespace Citrine;

/*************
 Vector2
 *************/

Citrine::Vector2::Vector2(const float x, const float y)
{
	this->x = x;
	this->y = y;
}

Vector2 Citrine::Vector2::GetAdd(const Vector2& other)
{
	Vector2 result(x + other.x, y + other.y);
	return result;
}

void Citrine::Vector2::Add(const Vector2& other)
{
	*this = GetAdd(other);
}

Vector2 Citrine::Vector2::GetConcatenate(const float other)
{
	Vector2 result(x * other, y * other);
	return result;
}

void Citrine::Vector2::Concatenate(const float other)
{
	*this = GetConcatenate(other);
}

void Citrine::Vector2::Set(const Vector2& other)
{
	x = other.x;
	y = other.y;
}

float* Citrine::Vector2::GetArray()
{
	float* array = (float*) malloc(sizeof(float) * 2);
	array[0] = x;
	array[1] = y;
	return array;
}

/*************
 Vector3
 *************/

Citrine::Vector3::Vector3(const float x, const float y, const float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3 Citrine::Vector3::GetAdd(const Vector3& other)
{
	Vector3 result(x + other.x, y + other.y, z + other.z);
	return result;
}

void Citrine::Vector3::Add(const Vector3& other)
{
	*this = GetAdd(other);
}

Vector3 Citrine::Vector3::GetConcatenate(const float other)
{
	Vector3 result(x * other, y * other, z * other);
	return result;
}

void Citrine::Vector3::Concatenate(const float other)
{
	*this = GetConcatenate(other);
}

void Citrine::Vector3::Set(const Vector3& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

float* Citrine::Vector3::GetArray()
{
	float* array = (float*) malloc(sizeof(float) * 3);
	array[0] = x;
	array[1] = y;
	array[2] = z;
	return array;
}

/*************
 Vector4
 *************/

Citrine::Vector4::Vector4(const float x, const float y, const float z, const float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4 Citrine::Vector4::GetAdd(const Vector4& other)
{
	Vector4 result(x + other.x, y + other.y, z + other.z, w + other.w);
	return result;
}

void Citrine::Vector4::Add(const Vector4& other)
{
	*this = GetAdd(other);
}

Vector4 Citrine::Vector4::GetConcatenate(const float other)
{
	Vector4 result(x * other, y * other, z * other, w * other);
	return result;
}

void Citrine::Vector4::Concatenate(const float other)
{
	*this = GetConcatenate(other);
}

void Citrine::Vector4::Set(const Vector4& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
}

float* Citrine::Vector4::GetArray()
{
	float* array = (float*) malloc(sizeof(float) * 4);
	array[0] = x;
	array[1] = y;
	array[2] = z;
	array[3] = w;
	return array;
}
