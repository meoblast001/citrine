/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CAUDIOINSTANCE_H
#define CAUDIOINSTANCE_H

#include <stdio.h>
#include "caudiocontext.h"
#include "../csmartpointer.h"
#include "caudiosource.h"

namespace Citrine
{
	class AudioInstance
	{
		public:
			AudioInstance(IAudioContext* iaudiocontext, SmartPointer<AudioSource> audiosource);
			~AudioInstance();
			void Play();
			void Stop();
			const char* GetFileName();
		private:
			char* filename;
			IAudioContext* iaudiocontext;
			const void* instance;
	};
}
#endif
