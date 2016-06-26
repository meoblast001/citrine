/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CCONTEXT_H
#define CCONTEXT_H

namespace Citrine
{
	typedef struct _PlatformContext* PlatformContext;
	class IGraphicsContext;
	class IEventContext;

	class IContext
	{
		public:
			IContext(unsigned int width, unsigned int height);
			~IContext();
			IGraphicsContext* GetGraphicsContext();
			IEventContext* GetEventContext();
		private:
			PlatformContext platform_context;
			IGraphicsContext* graphics_context;
			IEventContext* event_context;
	};
}
#endif
