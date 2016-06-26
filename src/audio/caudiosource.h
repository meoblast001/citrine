/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CAUDIOSOURCE_H
#define CAUDIOSOURCE_H

#include <stdio.h>

namespace Citrine
{
	class AudioSource
	{
		public:
			AudioSource(const char* filename);
			~AudioSource();
			const char* GetFileName();
		private:
			char* filename;
	};
}
#endif
