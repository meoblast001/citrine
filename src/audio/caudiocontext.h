/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CAUDIOCONTEXT_H
#define CAUDIOCONTEXT_H

#include <stdio.h>
#include <list>
#include "../csmartpointer.h"
#include "csoundsource.h"

using namespace std;

struct ALCcontext_struct;
struct ALCdevice_struct;

namespace Citrine
{
	class AudioSource;
	class AudioInstance;
	class VorbisAudioFile;

	class IAudioContext
	{
		public:
			IAudioContext();
			~IAudioContext();
			const void* AddAudio(AudioInstance* audioinstance);
			void RemoveAudio(AudioInstance* audioinstance);
			void PlayAudio(const void* instance);
			void StopAudio(const void* instance);
			void PlaySound2D(SmartPointer<SoundSource> sound_source);
			void Update();
		private:
			struct Instance
			{
				unsigned int buffers[4];
				unsigned int source;
				bool playing;
				AudioInstance* parent;
				/*For Vorbis*/
				VorbisAudioFile* audiofile;
			};

			struct SoundInstance
			{
				unsigned int buffer;
				unsigned int source;
			};

			ALCcontext_struct* context;
			ALCdevice_struct* device;
			std::list<Instance*> instances;
			std::list<SoundInstance*> sound_instances;

			bool Buffer(Instance* instance, unsigned int buffer);
			void Buffer(void* data, unsigned int size, unsigned rate, unsigned char channels, unsigned int buffer);
			void DeleteInstance(Instance* instance);
	};
}
#endif
