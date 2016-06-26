/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <stdlib.h>
#include <string.h>
#include "caudiosource.h"

Citrine::AudioSource::AudioSource(const char* filename)
{
	/*Copy file name*/
	this->filename = (char*) malloc(strlen(filename) + 1);
	strcpy(this->filename, filename);
}

Citrine::AudioSource::~AudioSource()
{
	free(filename);
}

const char* Citrine::AudioSource::GetFileName()
{
	return filename;
}
