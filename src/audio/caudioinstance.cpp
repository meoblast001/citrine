/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <stdlib.h>
#include <string.h>
#include "caudioinstance.h"

Citrine::AudioInstance::AudioInstance(IAudioContext* iaudiocontext, SmartPointer<AudioSource> audiosource)
{
	/*Store IAudioContext pointer*/
	this->iaudiocontext = iaudiocontext;
	/*Get file handle from AudioSource*/
	const char* filename = audiosource->GetFileName();
	this->filename = (char*) malloc(strlen(filename) + 1);
	strcpy(this->filename, filename);

	/*Add to IAudioContext*/
	instance = iaudiocontext->AddAudio(this);
}

Citrine::AudioInstance::~AudioInstance()
{
	iaudiocontext->RemoveAudio(this);
	free(filename);
}

void Citrine::AudioInstance::Play()
{
	iaudiocontext->PlayAudio(instance);
}

void Citrine::AudioInstance::Stop()
{
	iaudiocontext->StopAudio(instance);
}

const char* Citrine::AudioInstance::GetFileName()
{
	return filename;
}
