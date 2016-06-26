/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CXORG_H
#define CXORG_H

#include "../../../config.h"

#if HAVE_XORG
namespace Citrine
{
	struct _PlatformContext;
	struct PlatformEvent;
}

Citrine::_PlatformContext* _CreateContext(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
void _DeleteContext(Citrine::_PlatformContext* context);
void _SetCurrentContext(Citrine::_PlatformContext* context);
void _ContextSwapBuffers(Citrine::_PlatformContext* context);
bool _ContextGetEvent(Citrine::_PlatformContext* context, Citrine::PlatformEvent* event);
#endif
#endif
