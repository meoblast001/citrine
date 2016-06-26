/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CIMAGE_H
#define CIMAGE_H

struct SDL_Surface;

namespace Citrine
{
	class Image
	{
		public:
			~Image();
			const void* GetPixels();
			void LoadImage(const char* filename);
			unsigned int GetWidth();
			unsigned int GetHeight();
		private:
			void* pixels;
			unsigned int width;
			unsigned int height;
	};
}
#endif
