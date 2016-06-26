/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include "cendianness.h"

uint16_t Citrine::Endianness::ConvertBigEndian(uint16_t in)
{
#if HAVE_BIG_ENDIAN
	return in;
#else
	unsigned short out;
	char* in_ptr = (char*) &in;
	char* out_ptr = (char*) &out;

	out_ptr[0] = in_ptr[1];
	out_ptr[1] = in_ptr[0];
	return out;
#endif
}

uint32_t Citrine::Endianness::ConvertBigEndian(uint32_t in)
{
#if HAVE_BIG_ENDIAN
	return in;
#else
	unsigned int out;
	char* in_ptr = (char*) &in;
	char* out_ptr = (char*) &out;

	out_ptr[0] = in_ptr[3];
	out_ptr[1] = in_ptr[2];
	out_ptr[2] = in_ptr[1];
	out_ptr[3] = in_ptr[0];
	return out;
#endif
}

float Citrine::Endianness::ConvertBigEndian(float in)
{
#if HAVE_BIG_ENDIAN
	return in;
#else
	float out;
	char* in_ptr = (char*) &in;
	char* out_ptr = (char*) &out;

	out_ptr[0] = in_ptr[3];
	out_ptr[1] = in_ptr[2];
	out_ptr[2] = in_ptr[1];
	out_ptr[3] = in_ptr[0];
	return out;
#endif
}

uint16_t Citrine::Endianness::ConvertLittleEndian(uint16_t in)
{
#if HAVE_BIG_ENDIAN
	unsigned short out;
	char* in_ptr = (char*) &in;
	char* out_ptr = (char*) &out;

	out_ptr[0] = in_ptr[1];
	out_ptr[1] = in_ptr[0];
	return out;
#else
	return in;
#endif
}

uint32_t Citrine::Endianness::ConvertLittleEndian(uint32_t in)
{
#if HAVE_BIG_ENDIAN
	unsigned int out;
	char* in_ptr = (char*) &in;
	char* out_ptr = (char*) &out;

	out_ptr[0] = in_ptr[3];
	out_ptr[1] = in_ptr[2];
	out_ptr[2] = in_ptr[1];
	out_ptr[3] = in_ptr[0];
	return out;
#else
	return in;
#endif
}

float Citrine::Endianness::ConvertLittleEndian(float in)
{
#if HAVE_BIG_ENDIAN
	float out;
	char* in_ptr = (char*) &in;
	char* out_ptr = (char*) &out;

	out_ptr[0] = in_ptr[3];
	out_ptr[1] = in_ptr[2];
	out_ptr[2] = in_ptr[1];
	out_ptr[3] = in_ptr[0];
	return out;
#else
	return in;
#endif
}
