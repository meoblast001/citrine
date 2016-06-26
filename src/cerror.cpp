/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "cerror.h"

using namespace Citrine;

void Citrine::ErrorHandling::Error(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	fprintf(stderr, "ERROR: ");
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
	va_end(args);
	abort();
}

void Citrine::ErrorHandling::Warning(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	printf("ERROR: ");
	vprintf(format, args);
	printf("\n");
	va_end(args);
}
