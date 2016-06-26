/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CERROR_H
#define CERROR_H

namespace Citrine
{
	namespace ErrorHandling
	{
		void Error(const char* format, ...);
		void Warning(const char* format, ...);
	}
}
#endif
