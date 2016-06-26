/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <stdlib.h>
#include <string.h>
#include "cimage.h"
#include "cpng.h"

Citrine::Image::~Image()
{
	free(pixels);
}

const void* Citrine::Image::GetPixels()
{
	return pixels;
}

void Citrine::Image::LoadImage(const char* filename)
{
	/*If filename ends in ".png", load as PNG file*/
	if (!strncmp(filename + strlen(filename) - 4, ".png", 4))
	{
		PNGImage image = LoadPNG(filename);
		pixels = image.pixels;
		width = image.width;
		height = image.height;
	}
}

unsigned int Citrine::Image::GetWidth()
{
	return width;
}
unsigned int Citrine::Image::GetHeight()
{
	return height;
}
