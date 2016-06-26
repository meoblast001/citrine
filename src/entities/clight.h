/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CLIGHT_H
#define CLIGHT_H

#include "../math/cvector.h"

namespace Citrine
{
	class Light
	{
		public:
			Light();
			Vector4* GetPosition();
			Vector4* GetAmbient();
			Vector4* GetDiffuse();
			Vector4* GetSpecular();
		private:
			Vector4 position;
			Vector4 ambient;
			Vector4 diffuse;
			Vector4 specular;
	};
}
#endif
