/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CSPRITE3D_H
#define CSPRITE3D_H

#include "../images/ctexture.h"
#include "../math/ctransform.h"
#include "../csmartpointer.h"

namespace Citrine
{
	class Sprite3D
	{
		public:
			SmartPointer<Texture> GetTexture();
			void SetTexture(SmartPointer<Texture> texture);
			Transform* GetTransform();
		private:
			SmartPointer<Texture> texture;
			Transform transformation;
	};
}
#endif
