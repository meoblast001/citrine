/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CWAV_H
#define CWAV_H

#include <utility>

namespace Citrine
{
	class WAVAudioFile
	{
		public:
			WAVAudioFile(const char* filename);
			~WAVAudioFile();
			std::pair<void*, unsigned int> GetData();
			unsigned int GetRate();
			unsigned char GetChannels();
		private:
			void* data;
			unsigned int data_size;
			unsigned int rate;
			unsigned char channels;
	};
}
#endif
