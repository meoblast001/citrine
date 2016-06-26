/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <stdio.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include "cvorbis.h"
#include "../cerror.h"

using namespace Citrine;
using namespace ErrorHandling;

Citrine::VorbisAudioFile::VorbisAudioFile(const char* filename)
{
	//Open file
	FILE* file = fopen(filename, "rb");
	if (!file)
		Error("Vorbis file \"%s\" not found", filename);

	oggstream = new OggVorbis_File;
	ov_open(file, oggstream, NULL, 0);
	vorbisinfo = ov_info(oggstream, -1);
}

Citrine::VorbisAudioFile::~VorbisAudioFile()
{
	ov_clear(oggstream);
	delete oggstream;
}

bool Citrine::VorbisAudioFile::GetChunk(void* buffer, unsigned int size)
{
	unsigned int size_used = 0;
	int section;
	while (size_used < size)
	{
		//Read data from OGG stream
		int result = ov_read(oggstream, (char*) buffer + size_used, size - size_used, 0, 2, 1, &section);

		//If result shows a valid read, record amount read, else break
		if (result > 0)
			size_used += result;
		else
			break;
	}

	//If no data was read, return false, else, return true
	if (size_used == 0)
		return false;
	return true;
}

unsigned int Citrine::VorbisAudioFile::GetRate()
{
	return vorbisinfo->rate;
}

unsigned char Citrine::VorbisAudioFile::GetChannels()
{
	return vorbisinfo->channels;
}

void Citrine::VorbisAudioFile::Rewind()
{
	ov_raw_seek(oggstream, 0);
}
