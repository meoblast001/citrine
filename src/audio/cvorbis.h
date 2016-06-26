/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CVORBIS_H
#define CVORBIS_H

struct OggVorbis_File;
struct vorbis_info;

namespace Citrine
{
	class VorbisAudioFile
	{
		public:
			VorbisAudioFile(const char* filename);
			~VorbisAudioFile();
			bool GetChunk(void* buffer, unsigned int size);
			unsigned int GetRate();
			unsigned char GetChannels();
			void Rewind();
		private:
			OggVorbis_File* oggstream;
			vorbis_info* vorbisinfo;
	};
}
#endif
