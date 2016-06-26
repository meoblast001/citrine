/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include "../../../config.h"
#if HAVE_XORG
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>
#include "cxorg.h"
#include "ccrossplatform.h"
#include "../events/ckeys.h"

using namespace Citrine;

struct Citrine::_PlatformContext
{
	Display* display;
	Window window;
	GLXContext context;
};

static KeySym keytable[] = {XK_a, XK_b, XK_c, XK_d, XK_e, XK_f, XK_g, XK_h, XK_i, XK_j, XK_k, XK_l, XK_m, XK_n, XK_o, XK_p, XK_q, XK_r, XK_s, XK_t, XK_u, XK_v, XK_w, XK_x, XK_y, XK_z,
	XK_0, XK_1, XK_2, XK_3, XK_4, XK_5, XK_6, XK_7, XK_8, XK_9,
	XK_KP_0, XK_KP_1, XK_KP_2, XK_KP_3, XK_KP_4, XK_KP_5, XK_KP_6, XK_KP_7, XK_KP_8, XK_KP_9,
	XK_Left,
	XK_Up,
	XK_Right,
	XK_Down,
	XK_Return,
	XK_space,
	XK_Delete,
	XK_End,
	XK_Page_Down,
	XK_Page_Up,
	XK_Home,
	XK_Insert,
	XK_Shift_L,
	XK_Shift_R,
	XK_backslash,
	XK_BackSpace,
	XK_Caps_Lock,
	XK_Tab,
	XK_grave,
	XK_bracketleft,
	XK_bracketright,
	XK_semicolon,
	XK_apostrophe,
	XK_comma,
	XK_period,
	XK_slash,
	0};

static Key TranslateKey(KeySym xkeysym)
{
	/*Iterate through the keytable and Key enum until xkeysym is found*/
	Key out = KEY_A;
	for (unsigned int i = 0; keytable[i]; i++)
	{
		/*Return associated key*/
		if (xkeysym == keytable[i])
			return out;
		out = (Key) ((int) out + 1);
	}
	return (Key) 0;
}

Citrine::PlatformContext _CreateContext(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	_PlatformContext* context = new _PlatformContext;

	/*Open X display*/
	context->display = XOpenDisplay(0);

	/*Define GLX attributes*/
	static int attributes[] = {GLX_RGBA,
		GLX_DOUBLEBUFFER,
		GLX_DEPTH_SIZE, 16,
		GLX_RED_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		0};

	/*Find a visual meeting GLX attributes*/
	XVisualInfo* visual_info = glXChooseVisual(context->display, DefaultScreen(context->display), attributes);

	/*Define window attributes*/
	XSetWindowAttributes window_attributes;
	window_attributes.event_mask = ExposureMask | VisibilityChangeMask | KeyPressMask | PointerMotionMask | StructureNotifyMask;
	window_attributes.bit_gravity = StaticGravity;
	window_attributes.colormap = XCreateColormap(context->display, RootWindow(context->display, visual_info->screen), visual_info->visual, AllocNone);

	/*Define window mask attributes*/
	unsigned long window_mask = CWBorderPixel | CWBitGravity | CWEventMask | CWColormap;

	/*Create X window*/
	context->window = XCreateWindow(context->display, DefaultRootWindow(context->display), x, y, width, height, 0, visual_info->depth, InputOutput, visual_info->visual, window_mask, &window_attributes);
	/*Map window to display*/
	XMapWindow(context->display, context->window);

	/*Select input types used*/
	XSelectInput(context->display, context->window, KeyPressMask | KeyReleaseMask);

	/*Create GLX context*/
	context->context = glXCreateContext(context->display, visual_info, 0, True);

	return context;
}

void _DeleteContext(Citrine::PlatformContext context)
{
	//Swap buffers before destroying context
	_ContextSwapBuffers(context);

	/*Unbind context*/
	glXMakeCurrent(context->display, None, 0);
	/*Destroy context and window*/
	glXDestroyContext(context->display, context->context);
	XDestroyWindow(context->display, context->window);
	/*Close display*/
	XCloseDisplay(context->display);

	delete context;
}

void _SetCurrentContext(Citrine::PlatformContext context)
{
	glXMakeCurrent(context->display, context->window, context->context);
}

void _ContextSwapBuffers(Citrine::PlatformContext context)
{
	glXSwapBuffers(context->display, context->window);
}

bool _ContextGetEvent(Citrine::_PlatformContext* context, Citrine::PlatformEvent* event)
{
	/*Return if no events are pending*/
	if (!XPending(context->display))
	return false;

	/*Get next event*/
	XEvent xevent;
	XNextEvent(context->display, &xevent);

	switch (xevent.type)
	{
		case KeyPress:
		{
			event->type = EVENT_KEY_PRESS;
			KeySym xkeysym = XLookupKeysym(&xevent.xkey, 0);
			event->key = TranslateKey(xkeysym);
			break;
		}
		case KeyRelease:
		{
			event->type = EVENT_KEY_RELEASE;
			KeySym xkeysym = XLookupKeysym(&xevent.xkey, 0);
			event->key = TranslateKey(xkeysym);
			break;
		}
		default:
		{
			return false;
			break;
		}
	}

	return true;
}
#endif
