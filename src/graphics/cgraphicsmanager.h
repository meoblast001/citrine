/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CGRAPHICSMANAGER_H
#define CGRAPHICSMANAGER_H

namespace Citrine
{
	class GraphicsManager
	{
		public:
			GraphicsManager();
			SmartPointer<MeshData> CreateMeshData();
			SmartPointer<Sprite3D> CreateSprite3D();
			SmartPointer<Sprite2D> CreateSprite2D();
			SmartPointer<Shader> CreateShader();
			SmartPointer<ShaderProgram> CreateShaderProgram();
		private:
			static bool already_initialized;

			//Declared private to prevent copying or assignment; Not defined
			GraphicsManager(const GraphicsManager& other);
			operator=(const GraphicsManager& other);
	};
}
#endif
