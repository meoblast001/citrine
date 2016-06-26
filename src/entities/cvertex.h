/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CVERTEX_H
#define CVERTEX_H

namespace Citrine
{
	struct Vertex
	{
		Vector3 position;
		Vector2 texcoord;
		Vector4 weights;
		unsigned char armatures[4];
	};
}
#endif
