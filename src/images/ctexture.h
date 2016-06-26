/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CTEXTURE_H
#define CTEXTURE_H

#include "cimage.h"
#include "../csmartpointer.h"
#include "../contexts/graphics/cgraphicscontext.h"

typedef unsigned int GLuint;

namespace Citrine
{
	class Texture
	{
		public:
			Texture(IGraphicsContext* igraphicscontext);
			~Texture();
			SmartPointer<Image> GetImage();
			void SetImage(SmartPointer<Image> image);
			const void* GetBufferObject();
		private:
			SmartPointer<Image> image;
			IGraphicsContext* igraphicscontext;
			const void* buffer_object;
	};
}
#endif
