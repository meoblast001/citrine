/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include "csoundsource.h"
#include "cwav.h"

Citrine::SoundSource::SoundSource(const char* filename)
{
	audio = new WAVAudioFile(filename);
}

Citrine::SoundSource::~SoundSource()
{
	delete audio;
}

std::pair<void*, unsigned int> Citrine::SoundSource::GetData()
{
	return audio->GetData();
}

unsigned int Citrine::SoundSource::GetRate()
{
	return audio->GetRate();
}

unsigned char Citrine::SoundSource::GetChannels()
{
	return audio->GetChannels();
}
