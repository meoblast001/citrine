/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CARMATURE_H
#define CARMATURE_H

#include <utility>
#include "../math/ctransform.h"
#include "cvertex.h"

namespace Citrine
{
	struct Armature
	{
		Transform transformation;
		Armature* parent;
	};

	std::pair<Vertex*, unsigned short> ArmatureVertexDeform(std::pair<Vertex*, unsigned short> vertices, std::pair<Armature*, unsigned short> armatures);
}
#endif
