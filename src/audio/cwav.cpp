/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cwav.h"
#include "../cerror.h"
#include "../cendianness.h"

using namespace Citrine;
using namespace ErrorHandling;
using namespace Endianness;

static void ReadData(void* ptr, size_t size, size_t count, FILE* file)
{
	size_t result = fread(ptr, size, count, file);
	if (result != count)
		Error("WAV data could not be read\n");
}

Citrine::WAVAudioFile::WAVAudioFile(const char* filename)
{
	//Open file
	FILE* file = fopen(filename, "rb");
	if (!file)
		Error("WAV file \"%s\" not found", filename);

	//Header variables
	char chunk_id[4];
	uint32_t chunk_size;
	char format[4];

	//Read header variables
	ReadData(chunk_id, 1, 4, file);
	ReadData(&chunk_size, 4, 1, file);
	ReadData(&format, 1, 4, file);
	//Convert endianness
	chunk_size = ConvertLittleEndian(chunk_size);
	//Check ID and format
	if (strncmp(chunk_id, "RIFF", 4))
		Error("Chunk ID invalid for WAV file \"%s\"", filename);
	if (strncmp(format, "WAVE", 4))
		Error("Format invalid for WAV file \"%s\"", filename);

	//Format subchunk variables
	char subchunk1_id[4];
	uint32_t subchunk1_size;
	uint16_t audio_format;
	uint16_t num_channels;
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint16_t block_align;
	uint16_t bits_per_sample;

	//Read format subchunk variables
	ReadData(subchunk1_id, 1, 4, file);
	ReadData(&subchunk1_size, 4, 1, file);
	ReadData(&audio_format, 2, 1, file);
	ReadData(&num_channels, 2, 1, file);
	ReadData(&sample_rate, 4, 1, file);
	ReadData(&byte_rate, 4, 1, file);
	ReadData(&block_align, 2, 1, file);
	ReadData(&bits_per_sample, 2, 1, file);
	//Convert endianness
	subchunk1_size = ConvertLittleEndian(subchunk1_size);
	audio_format = ConvertLittleEndian(audio_format);
	num_channels = ConvertLittleEndian(num_channels);
	sample_rate = ConvertLittleEndian(sample_rate);
	byte_rate = ConvertLittleEndian(byte_rate);
	block_align = ConvertLittleEndian(block_align);
	bits_per_sample = ConvertLittleEndian(bits_per_sample);

	//Check ID and size
	if (strncmp(subchunk1_id, "fmt ", 4))
		Error("Format subchunk ID invalid for WAV file \"%s\"", filename);
	if (subchunk1_size != 16)
		Error("WAV file \"%s\" is not in PCM format", filename);

	//Data subchunk variables
	char subchunk2_id[4];
	uint32_t subchunk2_size;
	void* data;

	//Read data subchunk variables
	ReadData(subchunk2_id, 1, 4, file);
	ReadData(&subchunk2_size, 4, 1, file);
	//Convert endianness
	subchunk2_size = ConvertLittleEndian(subchunk2_size);

	//Check ID
	if (strncmp(subchunk2_id, "data", 4))
		Error("Data subchunk ID invalid for WAV file \"%s\"", filename);

	//Get data
	data = malloc(subchunk2_size);
	ReadData(data, 1, subchunk2_size, file);

	//Copy to class
	this->data = data;
	data_size = subchunk2_size;
	rate = sample_rate;
	channels = num_channels;

	fclose(file);
}

Citrine::WAVAudioFile::~WAVAudioFile()
{
	free(data);
}

std::pair<void*, unsigned int> Citrine::WAVAudioFile::GetData()
{
	return std::pair<void*, unsigned int> (data, data_size);
}

unsigned int Citrine::WAVAudioFile::GetRate()
{
	return rate;
}

unsigned char Citrine::WAVAudioFile::GetChannels()
{
	return channels;
}
