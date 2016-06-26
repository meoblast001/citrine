/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include "cpng.h"
#include "../cerror.h"

using namespace Citrine::ErrorHandling;

PNGImage LoadPNG(const char* filename)
{
	png_byte header[8];

	/*Open file and get header data*/
	FILE* file = fopen(filename, "rb");
	//If file handle is null, an error has occurred
	if (!file)
		Error("PNG file \"%s\" not found", filename);

	fread(header, 1, 8, file);
	//Check for valid PNG header
	if (png_sig_cmp(header, 0, 8))
		Warning("PNG header is not correct in PNG file \"%s\"", filename);

	/*Create PNG read struct*/
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

	png_init_io(png, file);
	png_set_sig_bytes(png, 8);

	/*Create PNG info struct*/
	png_infop info = png_create_info_struct(png);
	png_read_info(png, info);

	/*Construct output buffer*/
	png_byte* image_data = (png_byte*) malloc(info->height * info->rowbytes);

	/*Construct reference to each individual row in the output buffer*/
	png_bytep* image_rows = (png_bytep*) malloc(sizeof(png_bytep) * info->height);
	for (unsigned int i = 0; i < info->height; i++)
		image_rows[i] = image_data + (i * info->rowbytes);

	/*Read PNG file into output buffer*/
	png_read_image(png, image_rows);
	fclose(file);

	/*Convert to RGB 8-bit*/
	png_set_expand(png);
	if (info->bit_depth == 16)
		png_set_strip_16(png);
	if (info->color_type & PNG_COLOR_MASK_ALPHA)
		png_set_strip_alpha(png);

	/*Construct output PNGImage struct*/
	PNGImage output;
	output.pixels = image_data;
	output.width = info->width;
	output.height = info->height;

	return output;
}
