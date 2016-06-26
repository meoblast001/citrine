/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CENDIANNESS_H
#define CENDIANNESS_H

#include <stdint.h>

namespace Citrine
{
	namespace Endianness
	{
		uint16_t ConvertBigEndian(uint16_t in);
		uint32_t ConvertBigEndian(uint32_t in);
		float ConvertBigEndian(float in);
		uint16_t ConvertLittleEndian(uint16_t in);
		uint32_t ConvertLittleEndian(uint32_t in);
		float ConvertLittleEndian(float in);
	}
}
#endif
