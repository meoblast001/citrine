/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CCROSSPLATFORM_H
#define CCROSSPLATFORM_H

#include <list>
#include "../events/ckeys.h"

namespace Citrine
{
	typedef struct _PlatformContext* PlatformContext;

	enum EventType
	{
		EVENT_KEY_PRESS,
		EVENT_KEY_RELEASE
	};

	struct PlatformEvent
	{
		EventType type;
		Key key;
	};

	PlatformContext CreateContext(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	void DeleteContext(PlatformContext context);
	void SetCurrentContext(PlatformContext context);
	void ContextSwapBuffers(PlatformContext context);
	bool ContextGetEvent(PlatformContext context, PlatformEvent* event);
}
#endif
