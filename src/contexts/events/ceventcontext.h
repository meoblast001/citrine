/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CEVENTPROC_H
#define CEVENTPROC_H

#include "ckeys.h"

namespace Citrine
{
	struct _PlatformContext;

	class IEventContext
	{
		public:
			IEventContext(_PlatformContext* platform_context);
			~IEventContext();
			void ProcessEvents();
			char GetKeyState(Key key);
			bool GetQuit();
		private:
			_PlatformContext* platform_context;
			bool* keys;
			bool quit;
	};
}
#endif
