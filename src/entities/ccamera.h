/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CCAMERA_H
#define CCAMERA_H

#include "../math/ctransform.h"

namespace Citrine
{
	class Camera
	{
		public:
			Transform* GetTransformation();
		private:
			Transform transformation;
	};
}
#endif
