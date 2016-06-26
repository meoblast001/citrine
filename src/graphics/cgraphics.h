/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CGRAPHICS_H
#define CGRAPHICS_H

namespace Citrine
{
	class Graphics
	{
		public:
			typedef struct _VBO* VBO;
			typedef struct _Texture* Texture;

			Graphics();
			const VBO CreateVBO(const Vertex* vertices, unsigned short n_vertices, const Vertex* triangles, unsigned short n_triangles);
			void DeleteVBO(const VBO vbo);
			const Texture CreateTexture(unsigned short width, unsigned short height, const void* pixels);
			void DeleteTexture(const Texture texture);
		private:
			static bool already_initialized;
			bool have_vbo;

			//Declared private to prevent copying or assignment; Not defined
			Graphics(const Graphics& other);
			operator=(const Graphics& other);
	};
}
#endif
