/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include "carmature.h"
#include "../math/cvector.h"

using namespace Citrine;

std::pair<Vertex*, unsigned short> Citrine::ArmatureVertexDeform(std::pair<Vertex*, unsigned short> vertices, std::pair<Armature*, unsigned short> armatures)
{
	//Allocate space for new vertices
	std::pair<Vertex*, unsigned short> results;
	results.first = new Vertex[vertices.second];
	results.second = vertices.second;

	for (unsigned short i = 0; i < vertices.second; i++)
	{
		//Construct Vector3 out of current vertex
		Vector3 current_vert;
		current_vert.x = vertices.first[i].position.x;
		current_vert.y = vertices.first[i].position.y;
		current_vert.z = vertices.first[i].position.z;

		//If weights sum to 0, there is no armature influence
		if (vertices.first[i].weights.x + vertices.first[i].weights.y + vertices.first[i].weights.z + vertices.first[i].weights.w == 0.0f)
		{
			results.first[i].position.x = current_vert.x;
			results.first[i].position.y = current_vert.y;
			results.first[i].position.z = current_vert.z;
		}
		else
		{
			Vector3 result;
			result.x = 0.0f;
			result.y = 0.0f;
			result.z = 0.0f;

			//Perform (armature * vertex) * weight for each valid armature
			if (vertices.first[i].armatures[0] > 0)
				result += (armatures.first[vertices.first[i].armatures[0] - 1].transformation.GetRotationMatrix() * current_vert) * vertices.first[i].weights.x;
			if (vertices.first[i].armatures[1] > 0)
				result += (armatures.first[vertices.first[i].armatures[1] - 1].transformation.GetRotationMatrix() * current_vert) * vertices.first[i].weights.y;
			if (vertices.first[i].armatures[2] > 0)
				result += (armatures.first[vertices.first[i].armatures[2] - 1].transformation.GetRotationMatrix() * current_vert) * vertices.first[i].weights.z;
			if (vertices.first[i].armatures[3] > 0)
				result += (armatures.first[vertices.first[i].armatures[3] - 1].transformation.GetRotationMatrix() * current_vert) * vertices.first[i].weights.w;

			//Copy result vector to result vertex
			results.first[i].position.x = result.x;
			results.first[i].position.y = result.y;
			results.first[i].position.z = result.z;
			//Copy unchanged vertex values
			results.first[i].texcoord.x = vertices.first[i].texcoord.x;
			results.first[i].texcoord.y = vertices.first[i].texcoord.y;
		}
	}
	return results;
}
