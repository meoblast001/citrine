/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include "ccrossplatform.h"
#include "cxorg.h"

using namespace Citrine;

PlatformContext Citrine::CreateContext(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	return _CreateContext(x, y, width, height);
}

void Citrine::DeleteContext(PlatformContext context)
{
	_DeleteContext(context);
}

void Citrine::SetCurrentContext(PlatformContext context)
{
	_SetCurrentContext(context);
}

void Citrine::ContextSwapBuffers(PlatformContext context)
{
	_ContextSwapBuffers(context);
}

bool Citrine::ContextGetEvent(PlatformContext context, PlatformEvent* event)
{
	_ContextGetEvent(context, event);
}
