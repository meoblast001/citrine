/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CSOUNDSOURCE_H
#define CSOUNDSOURCE_H

#include <utility>

namespace Citrine
{
	class WAVAudioFile;

	class SoundSource
	{
		public:
			SoundSource(const char* filename);
			~SoundSource();
			std::pair<void*, unsigned int> GetData();
			unsigned int GetRate();
			unsigned char GetChannels();
		private:
			WAVAudioFile* audio;
	};
}
#endif
