/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CPNG_H
#define CPNG_H

struct PNGImage
{
	void* pixels;
	unsigned int width;
	unsigned int height;
};

PNGImage LoadPNG(const char* filename);
#endif
