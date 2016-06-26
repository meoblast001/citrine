/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include <stdlib.h>
#include <string.h>
#include "ceventcontext.h"
#include "../crossplatform/ccrossplatform.h"

Citrine::IEventContext::IEventContext(_PlatformContext* platform_context)
{
	this->platform_context = platform_context;
	keys = (bool*) malloc(KEY_ENUM_TERMINATION);
	/*Set all keys to unpressed*/
	memset(keys, false, KEY_ENUM_TERMINATION);
	quit = false;
}

Citrine::IEventContext::~IEventContext()
{
	free(keys);
}

void Citrine::IEventContext::ProcessEvents()
{
	PlatformEvent event;
	while (ContextGetEvent(platform_context, &event))
	{
		switch (event.type)
		{
			case EVENT_KEY_PRESS:
			{
				keys[event.key] = true;
				break;
			}
			case EVENT_KEY_RELEASE:
			{
				keys[event.key] = false;
				break;
			}
		}
	}
}

char Citrine::IEventContext::GetKeyState(Key key)
{
	return keys[key];
}
bool Citrine::IEventContext::GetQuit()
{
	return quit;
}
