/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <AL/al.h>
#include <AL/alc.h>
#include "caudiocontext.h"
#include "caudioinstance.h"
#include "cvorbis.h"

#define BUFFER_SIZE 32768

Citrine::IAudioContext::IAudioContext()
{
	/*Initialize OpenAL*/
	device = alcOpenDevice(0);
	context = alcCreateContext(device, 0);
	alcMakeContextCurrent(context);
}

Citrine::IAudioContext::~IAudioContext()
{
	for (std::list<Instance*>::iterator i = instances.begin(); i != instances.end(); i++)
		DeleteInstance(*i);

	alcDestroyContext(context);
	alcCloseDevice(device);
}

const void* Citrine::IAudioContext::AddAudio(AudioInstance* audioinstance)
{
	Instance* instance = new Instance;

	alGenBuffers(4, instance->buffers);
	alGenSources(1, &instance->source);

	alSource3f(instance->source, AL_POSITION, 0.0f, 0.0f, 0.0f);
	alSource3f(instance->source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alSource3f(instance->source, AL_DIRECTION, 0.0f, 0.0f, 0.0f);
	alSourcef(instance->source, AL_ROLLOFF_FACTOR, 0.0f);
	alSourcei(instance->source, AL_SOURCE_RELATIVE, AL_TRUE);

	instance->audiofile = new VorbisAudioFile(audioinstance->GetFileName());

	Buffer(instance, instance->buffers[0]);
	Buffer(instance, instance->buffers[1]);
	Buffer(instance, instance->buffers[2]);
	Buffer(instance, instance->buffers[3]);

	alSourceQueueBuffers(instance->source, 4, instance->buffers);
	alSourcePlay(instance->source);

	/*Add audioinstance pointer to instance*/
	instance->parent = audioinstance;

	/*Add instance to list*/
	instances.push_back(instance);

	return instance;
}

void Citrine::IAudioContext::RemoveAudio(AudioInstance* audioinstance)
{
	/*Search for instance in list*/
	for (std::list<Instance*>::iterator i = instances.begin(); i != instances.end(); i++)
	{
		/*If the texture is equal to the iterator, delete the TBO and remove it from the list*/
		if (audioinstance == (*i)->parent)
		{
			/*Delete Instance and remove it from the list*/
			DeleteInstance(*i);
			instances.erase(i);
		}
	}
}

void Citrine::IAudioContext::PlayAudio(const void* instance)
{
	Instance* typecast_instance = (Instance*) instance;
	typecast_instance->playing = true;
}

void Citrine::IAudioContext::StopAudio(const void* instance)
{
	Instance* typecast_instance = (Instance*) instance;
	typecast_instance->playing = false;
}

void Citrine::IAudioContext::PlaySound2D(SmartPointer<SoundSource> sound_source)
{
	SoundInstance* instance = new SoundInstance;

	//Generate buffer and source
	alGenBuffers(1, &instance->buffer);
	alGenSources(1, &instance->source);

	//Set default source properties for 2D sound
	alSource3f(instance->source, AL_POSITION, 0.0f, 0.0f, 0.0f);
	alSource3f(instance->source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alSource3f(instance->source, AL_DIRECTION, 0.0f, 0.0f, 0.0f);
	alSourcef(instance->source, AL_ROLLOFF_FACTOR, 0.0f);
	alSourcei(instance->source, AL_SOURCE_RELATIVE, AL_TRUE);

	//Buffer
	std::pair<void*, unsigned int> data = sound_source->GetData();
	Buffer(data.first, data.second, sound_source->GetRate(), sound_source->GetChannels(), instance->buffer);

	//Queue buffer and play source
	alSourceQueueBuffers(instance->source, 1, &instance->buffer);
	alSourcePlay(instance->source);

	sound_instances.push_back(instance);
}

void Citrine::IAudioContext::Update()
{
	for (std::list<Instance*>::iterator i = instances.begin(); i != instances.end(); i++)
	{
		/*Get amount of processed buffers*/
		int processed;
		alGetSourcei((*i)->source, AL_BUFFERS_PROCESSED, &processed);

		/*Unqueue, refresh, and queue processed buffers*/
		while (processed > 0)
		{
			ALuint current_buffer;

			alSourceUnqueueBuffers((*i)->source, 1, &current_buffer);
			if (!Buffer(*i, current_buffer))
			{
				/*Rewind if a buffer fails*/
				(*i)->audiofile->Rewind();
				Buffer(*i, current_buffer);
			}
			alSourceQueueBuffers((*i)->source, 1, &current_buffer);

			processed--;
		}
	}

	for (std::list<SoundInstance*>::iterator i = sound_instances.begin(); i != sound_instances.end(); i++)
	{
		//Get amount of processed buffers
		int processed;
		alGetSourcei((*i)->source, AL_BUFFERS_PROCESSED, &processed);

		//Unqueue
		if (processed)
		{
			ALuint current_buffer;
			alSourceUnqueueBuffers((*i)->source, 1, &current_buffer);
			alDeleteSources(1, &(*i)->source);
			alDeleteBuffers(1, &(*i)->buffer);
			delete *i;
			sound_instances.erase(i);
			i = sound_instances.begin();
		}
	}
}

bool Citrine::IAudioContext::Buffer(Instance* instance, unsigned int buffer)
{
	char data[BUFFER_SIZE];
	if (!instance->audiofile->GetChunk(data, BUFFER_SIZE))
		return false;

	/*Set format*/
	ALenum format;
	if (instance->audiofile->GetChannels() == 2)
		format = AL_FORMAT_STEREO16;
	else
		format = AL_FORMAT_MONO16;

	alBufferData(buffer, format, data, BUFFER_SIZE, instance->audiofile->GetRate());
	return true;
}

void Citrine::IAudioContext::Buffer(void* data, unsigned int size, unsigned rate, unsigned char channels, unsigned int buffer)
{
	ALenum format;
	if (channels == 2)
		format = AL_FORMAT_STEREO16;
	else
		format = AL_FORMAT_MONO16;

	alBufferData(buffer, format, data, size, rate);
}

void Citrine::IAudioContext::DeleteInstance(Instance* instance)
{
	/*If audio is playing, stop it*/
	if (instance->playing)
		alSourceStop(instance->source);

	/*Empty buffers*/
	int queued;
	alGetSourcei(instance->source, AL_BUFFERS_QUEUED, &queued);
	while (queued > 0)
	{
		alSourceUnqueueBuffers(instance->source, 4, instance->buffers);
		queued--;
	}

	alDeleteSources(1, &instance->source);
	alDeleteBuffers(4, instance->buffers);

	delete instance->audiofile;

	/*Delete instance*/
	delete instance;
}
