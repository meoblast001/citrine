/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include "ctransform.h"
#include <math.h>
#include <stdlib.h>

using namespace Citrine;

Citrine::Transform::Transform()
{
	tmatrix.x = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
	tmatrix.y = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	tmatrix.z = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	tmatrix.w = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4* Citrine::Transform::GetMatrix()
{
	return &tmatrix;
}

Matrix3x3 Citrine::Transform::GetRotationMatrix()
{
	/*Extract the top-left 3x3 from this transformation matrix*/
	Matrix3x3 matrix;
	matrix.x = Vector3(tmatrix.x.x, tmatrix.x.y, tmatrix.x.z);
	matrix.y = Vector3(tmatrix.y.x, tmatrix.y.y, tmatrix.y.z);
	matrix.z = Vector3(tmatrix.z.x, tmatrix.z.y, tmatrix.z.z);
	return matrix;
}

void Citrine::Transform::SetRotationMatrix(Matrix3x3* matrix)
{
	/*Set the top-left 3x3 of this transformation matrix*/
	tmatrix.x.x = matrix->x.x;
	tmatrix.x.y = matrix->x.y;
	tmatrix.x.z = matrix->x.z;
	tmatrix.y.x = matrix->y.x;
	tmatrix.y.y = matrix->y.y;
	tmatrix.y.z = matrix->y.z;
	tmatrix.z.x = matrix->z.x;
	tmatrix.z.y = matrix->z.y;
	tmatrix.z.z = matrix->z.z;
}

void Citrine::Transform::RotateOnXAxis(float xrot)
{
	Matrix3x3 tmprotmat;
	tmprotmat.x = Vector3(1.0f, 0.0f, 0.0f);
	tmprotmat.y = Vector3(0.0f, cos(xrot), -1.0f * sin(xrot));
	tmprotmat.z = Vector3(0.0f, sin(xrot), cos(xrot));
	/*Concatenate and Set*/
	SetRotationMatrix(&(tmprotmat * GetRotationMatrix()));
}

void Citrine::Transform::RotateOnYAxis(float yrot)
{
	Matrix3x3 tmprotmat;
	tmprotmat.x = Vector3(cos(yrot), 0.0f, sin(yrot));
	tmprotmat.y = Vector3(0.0f, 1.0f, 0.0f);
	tmprotmat.z = Vector3(-1.0f * sin(yrot), 0.0f, cos(yrot));
	/*Concatenate and Set*/
	SetRotationMatrix(&(tmprotmat * GetRotationMatrix()));
}

void Citrine::Transform::RotateOnZAxis(float zrot)
{
	Matrix3x3 tmprotmat;
	tmprotmat.x = Vector3(cos(zrot), -1.0f * sin(zrot), 0.0f);
	tmprotmat.y = Vector3(sin(zrot), cos(zrot), 0.0f);
	tmprotmat.z = Vector3(0.0f, 0.0f, 1.0f);
	/*Concatenate and Set*/
	SetRotationMatrix(&(tmprotmat * GetRotationMatrix()));
}

void Citrine::Transform::SetPositionToOther(Vector3* pos)
{
	tmatrix.w.x = pos->x;
	tmatrix.w.y = pos->y;
	tmatrix.w.z = pos->z;
}

void Citrine::Transform::SetPosition(float xpos, float ypos, float zpos)
{
	tmatrix.w.x = xpos;
	tmatrix.w.y = ypos;
	tmatrix.w.z = zpos;
}

void Citrine::Transform::SetPositionX(float xpos)
{
	tmatrix.w.x = xpos;
}

void Citrine::Transform::SetPositionY(float ypos)
{
	tmatrix.w.y = ypos;
}

void Citrine::Transform::SetPositionZ(float zpos)
{
	tmatrix.w.z = zpos;
}

Vector3 Citrine::Transform::GetPosition()
{
	/*Extract the position of this transformation matrix*/
	Vector3 result;
	result = Vector3(tmatrix.w.x, tmatrix.w.y, tmatrix.w.z);
	return result;
}

void Citrine::Transform::MovePosition(float x, float y, float z)
{
	tmatrix.w.x += x;
	tmatrix.w.y += y;
	tmatrix.w.z += z;
}

void Citrine::Transform::MovePositionToOther(Vector3* pos)
{
	MovePosition(pos->x, pos->y, pos->z);
}

void Citrine::Transform::MovePositionRelative(float x, float y, float z)
{
	MovePosition(tmatrix.x.x * x, tmatrix.x.y * x, tmatrix.x.z * x);
	MovePosition(tmatrix.y.x * y, tmatrix.y.y * y, tmatrix.y.z * y);
	MovePosition(tmatrix.z.x * z, tmatrix.z.y * z, tmatrix.z.z * z);
}

void Citrine::Transform::MovePositionRelativeToOther(Vector3* pos)
{
	MovePositionRelative(pos->x, pos->y, pos->z);
}
