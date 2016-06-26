/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include "ccontext.h"
#include "graphics/cgraphicscontext.h"
#include "events/ceventcontext.h"
#include "crossplatform/ccrossplatform.h"

using namespace Citrine;

Citrine::IContext::IContext(unsigned int width, unsigned int height)
{
	/*Create platform-specific context*/
	platform_context = CreateContext(20, 20, width, height);

	/*Create graphics and event contexts*/
	graphics_context = new IGraphicsContext(platform_context, width, height);
	event_context = new IEventContext(platform_context);
}

Citrine::IContext::~IContext()
{
	/*Delete graphics and event contexts*/
	delete graphics_context;
	delete event_context;

	/*Delete platform-specific context*/
	DeleteContext(platform_context);
}

IGraphicsContext* Citrine::IContext::GetGraphicsContext()
{
	return graphics_context;
}

IEventContext* Citrine::IContext::GetEventContext()
{
	return event_context;
}
