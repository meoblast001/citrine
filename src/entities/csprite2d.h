/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CSPRITE2D_H
#define CSPRITE2D_H

#include "../images/ctexture.h"
#include "../math/cvector.h"
#include "../csmartpointer.h"

namespace Citrine
{
	class Sprite2D
	{
		public:
			SmartPointer<Texture> GetTexture();
			void SetTexture(SmartPointer<Texture> texture);
			Vector2* GetPosition();
		private:
			SmartPointer<Texture> texture;
			Vector2 position;
	};
}
#endif
