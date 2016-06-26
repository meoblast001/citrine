/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <stdlib.h>
#include <string.h>
#include "cmatrix.h"

using namespace Citrine;

/***************
 Matrix2x2
 ***************/

Citrine::Matrix2x2::Matrix2x2(const Vector2& x, const Vector2& y)
{
	this->x = x;
	this->y = y;
}

Matrix2x2 Citrine::Matrix2x2::GetConcatenate(const Matrix2x2& other)
{
	Matrix2x2 result;

	memset(&result, 0, sizeof(Matrix2x2));

	/*Construct references to the beginning value of each matrix*/
	float* input1 = (float*) this;
	const float* input2 = (const float*) &other;
	float* output = (float*) &result;

	for (unsigned int i = 0; i < 2; i++)
		for (unsigned int j = 0; j < 2; j++)
			for (unsigned int k = 0; k < 2; k++)
				output[(i * 2) + j] += input1[(i * 2) + k] * input2[(k * 2) + j];

	return result;
}

void Citrine::Matrix2x2::Concatenate(const Matrix2x2& other)
{
	/*Multiply this matrix by the other matrix*/
	*this = GetConcatenate(other);
}

Matrix2x2 Citrine::Matrix2x2::GetConcatenate(const float other)
{
	Matrix2x2 result(x.GetConcatenate(other), y.GetConcatenate(other));
	return result;
}

void Citrine::Matrix2x2::Concatenate(const float other)
{
	/*Multiply this matrix by the scalar*/
	*this = GetConcatenate(other);
}

float* Citrine::Matrix2x2::GetArray()
{
	/*Construct a C array containing the values of the matrix*/
	float* array = (float*) malloc(sizeof(float) * 4);
	array[0] = x.x;
	array[1] = x.y;

	array[2] = y.x;
	array[3] = y.y;
	return array;
}

void Citrine::Matrix2x2::Set(const Matrix2x2& other)
{
	x = other.x;
	y = other.y;
}

/***************
 Matrix3x3
 ***************/

Citrine::Matrix3x3::Matrix3x3(const Vector3& x, const Vector3& y, const Vector3& z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Matrix3x3 Citrine::Matrix3x3::GetConcatenate(const Matrix3x3& other)
{
	Matrix3x3 result;

	memset(&result, 0, sizeof(Matrix3x3));

	/*Construct references to the beginning value of each matrix*/
	float* input1 = (float*) this;
	const float* input2 = (const float*) &other;
	float* output = (float*) &result;

	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			for (unsigned int k = 0; k < 3; k++)
				output[(i * 3) + j] += input1[(i * 3) + k] * input2[(k * 3) + j];

	return result;
}

void Citrine::Matrix3x3::Concatenate(const Matrix3x3& other)
{
	/*Multiply this matrix by the other matrix*/
	*this = GetConcatenate(other);
}

Matrix3x3 Citrine::Matrix3x3::GetConcatenate(const float other)
{
	Matrix3x3 result(x.GetConcatenate(other), y.GetConcatenate(other), z.GetConcatenate(other));
	return result;
}

void Citrine::Matrix3x3::Concatenate(const float other)
{
	/*Multiply this matrix by the scalar*/
	*this = GetConcatenate(other);
}

Vector3 Citrine::Matrix3x3::GetConcatenate(const Vector3& other)
{
	Vector3 result;
	result.x = other.x * x.x + other.y * x.y + other.z * x.z;
	result.y = other.x * y.x + other.y * y.y + other.z * y.z;
	result.z = other.x * z.x + other.z * z.y + other.z * z.z;
	return result;
}

float* Citrine::Matrix3x3::GetArray()
{
	/*Construct a C array containing the values of the matrix*/
	float* array = (float*) malloc(sizeof(float) * 9);
	array[0] = x.x;
	array[1] = x.y;
	array[2] = x.z;

	array[3] = y.x;
	array[4] = y.y;
	array[5] = y.z;

	array[6] = z.x;
	array[7] = z.y;
	array[8] = z.z;
	return array;
}

void Citrine::Matrix3x3::Set(const Matrix3x3& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

/***************
 Matrix4x4
 ***************/

Citrine::Matrix4x4::Matrix4x4(const Vector4& x, const Vector4& y, const Vector4& z, const Vector4& w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Matrix4x4 Citrine::Matrix4x4::GetConcatenate(const Matrix4x4& other)
{
	Matrix4x4 result;

	memset(&result, 0, sizeof(Matrix4x4));

	/*Construct references to the beginning value of each matrix*/
	float* input1 = (float*) this;
	const float* input2 = (const float*) &other;
	float* output = (float*) &result;

	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < 4; j++)
			for (unsigned int k = 0; k < 4; k++)
				output[(i * 4) + j] += input1[(i * 4) + k] * input2[(k * 4) + j];

	return result;
}

void Citrine::Matrix4x4::Concatenate(const Matrix4x4& other)
{
	/*Multiply this matrix by the other matrix*/
	*this = GetConcatenate(other);
}

Matrix4x4 Citrine::Matrix4x4::GetConcatenate(const float other)
{
	Matrix4x4 result(x.GetConcatenate(other), y.GetConcatenate(other), z.GetConcatenate(other), w.GetConcatenate(other));
	return result;
}

void Citrine::Matrix4x4::Concatenate(float other)
{
	/*Multiply this matrix by the scalar*/
	*this = GetConcatenate(other);
}

float* Citrine::Matrix4x4::GetArray()
{
	/*Construct a C array containing the values of the matrix*/
	float* array = (float*) malloc(sizeof(float) * 16);
	array[0] = x.x;
	array[1] = x.y;
	array[2] = x.z;
	array[3] = x.w;

	array[4] = y.x;
	array[5] = y.y;
	array[6] = y.z;
	array[7] = y.w;

	array[8] = z.x;
	array[9] = z.y;
	array[10] = z.z;
	array[11] = z.w;

	array[12] = w.x;
	array[13] = w.y;
	array[14] = w.z;
	array[15] = w.w;
	return array;
}

void Citrine::Matrix4x4::Set(const Matrix4x4& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

float Citrine::Matrix4x4::GetDeterminant()
{
	return x.x * y.y * z.z * w.w - x.y * y.x * z.z * w.w - x.w * x.y * z.z * w.x - x.x * y.z * z.y * w.w - x.x * y.y * z.w * w.z + x.y * y.z * z.x * w.w + x.w * y.y * z.x * w.z + x.w * y.x * z.z * w.y + x.x * y.z * z.w * w.y + x.z * y.x * z.y * w.w + x.z * y.y * z.w * w.x + x.x * y.w * z.y * w.z + x.y * y.w * z.z * w.x + x.y * y.x * z.w * w.z + x.w * y.z * z.y * w.x - x.y * y.z * z.w * w.x - x.w * y.x * z.y * w.z - x.y * y.w * z.x * w.z - x.z * y.x * z.w * w.y - x.z * y.w * z.y * w.x - x.w * y.z * z.x * w.w - x.x * y.w * z.z * w.y - x.z * y.y * z.x * w.w + x.z * y.w * z.x * w.y;
}

Matrix4x4 Citrine::Matrix4x4::GetInverse()
{
	Matrix4x4 inverse;
	inverse.x.x = y.y * z.z * w.w - y.y * z.w * w.z - y.z * z.y * w.w + y.z * z.w * w.y + y.w * z.y * w.z - y.w * z.z * w.y;
	inverse.y.x = -1.0f * (y.x * z.z * w.w - y.x * z.w * w.z - y.z * z.x * w.w + y.z * z.w * w.x + y.w * z.x * w.z - y.w * z.z * w.x);
	inverse.z.x = y.x * z.y * w.w - y.x * z.w * w.y - y.y * z.x * w.w + y.y * z.w * w.x + y.w * z.x * w.y - y.w * z.y * w.x;
	inverse.w.x = -1.0f * (y.x * z.y * w.z - y.x * z.z * w.y - y.y * z.x * w.z + y.y * z.z * w.x + y.z * z.x * w.y - y.z * z.y * w.x);

	inverse.x.y = -1.0f * (x.y * z.z * w.w - x.y * z.w * w.z - x.z * z.y * w.w + x.z * z.w * w.y + x.w * z.y * w.z - x.w * z.z * w.y);
	inverse.y.y = x.x * z.z * w.w - x.x * z.w * w.z - x.z * z.x * w.w + x.z * z.w * w.x + x.w * z.x * w.z - x.w * z.z * w.x;
	inverse.z.y = -1.0f * (x.x * z.y * w.w - x.x * z.w * w.y - x.y * z.x * w.w + x.y * z.w * w.x + x.w * z.x * w.y - x.w * z.y * w.x);
	inverse.w.y = x.x * z.y * w.z - x.x * z.z * w.y - x.y * z.x * w.z + x.y * z.z * w.x + x.z * z.x * w.y - x.z * z.y * w.x;

	inverse.x.z = x.y * y.z * w.w - x.y * y.w * w.z - x.z * y.y * w.w + x.z * y.w * w.y + x.w * y.y * w.z - x.w * y.z * w.y;
	inverse.y.z = -1.0f * (x.x * y.z * w.w - x.x * y.w * w.z - x.z * y.x * w.w + x.z * y.w * w.x + x.w * y.x * w.z - x.w * y.z * w.x);
	inverse.z.z = x.x * y.y * w.w - x.x * y.w * w.y - x.y * y.x * w.w + x.y * y.w * w.x + x.w * y.x * w.y - x.w * y.y * w.x;
	inverse.w.z = -1.0f * (x.x * y.y * w.z - x.x * y.z * w.y - x.y * y.x * w.z + x.y * y.z * w.x + x.z * y.x * w.y - x.z * y.y * w.x);

	inverse.x.w = -1.0f * (x.y * y.z * z.w - x.y * y.w * z.z - x.z * y.y * z.w + x.z * y.w * z.y + x.w * y.y * z.z - x.w * y.z * z.y);
	inverse.y.w = x.x * y.z * z.w - x.x * y.w * z.z - x.z * y.x * z.w + x.z * y.w * z.x + x.w * y.x * z.z - x.w * y.z * z.x;
	inverse.z.w = -1.0f * (x.x * y.y * z.w - x.x * y.w * z.y - x.y * y.x * z.w + x.y * y.w * z.x + x.w * y.x * z.y - x.w * y.y * z.x);
	inverse.w.w = x.x * y.y * z.z - x.x * y.z * z.y - x.y * y.x * z.z + x.y * y.z * z.x + x.z * y.x * z.y - x.z * y.y * z.x;

	float detval = inverse.GetDeterminant();
	if (detval != 0)
		detval = 1 / detval;

	inverse.Concatenate(detval);
	return inverse;
}
