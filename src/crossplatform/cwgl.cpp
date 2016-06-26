/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include "../../../config.h"
#if HAVE_NT
#include <windows.h>
#include "ccrossplatform.h"

using namespace Citrine;

struct Citrine::_PlatformContext
{
	HINSTANCE instance;
	HDC device;
	HWND window;
	HGLRC context;
};

LRESULT CALLBACK WndProc (HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case WM_CREATE:
			break;
		case WM_CLOSE:
			DestroyWindow(window);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(window, msg, wparam, lparam);
	}
	return 0L;
}

Citrine::PlatformContext _CreateContext(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	_PlatformContext* context = new _PlatformContext;

	//Get HINSTANCE
	context->instance = GetModuleHandle(NULL);

	//Set class properties
	WNDCLASS wndclass;
	memset(&wndclass, 0, sizeof(WNDCLASS));
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = context->instance;
	wndclass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "class";
	//Register class
	RegisterClass(&wndclass);

	//Create window
	context->window = CreateWindow("class", "Window", WS_OVERLAPPEDWINDOW, x, y, width, height, 0, 0, context->instance, 0);

	//Get device context
	context->device = GetDC(context->window);

	//Describe pixel format descriptor
	PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR),
		1, //Version
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32, //32-bit color
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		16, //Depth buffer size
		0, 0,
		PFD_MAIN_PLANE,
		0, 0, 0, 0};

	//Set pixel format descriptor
	SetPixelFormat(context->device, ChoosePixelFormat(context->device, &pfd), &pfd);

	//Create WGL context;
	context->context = wglCreateContext(context->device);

	//Make new context current
	wglMakeCurrent(context->device, context->context);

	ShowWindow(context->window, SW_SHOW);
	SetForegroundWindow(context->window);
	SetFocus(context->window);
	UpdateWindow(context->window);

	the_context = context;

	return context;
}

void _DeleteContext(Citrine::_PlatformContext* context)
{
	wglMakeCurrent(context->device, 0);
	wglDeleteContext(context->context);

	delete context;
}

void _SetCurrentContext(Citrine::_PlatformContext* context)
{
	wglMakeCurrent(context->device, context->context);
}

void _ContextSwapBuffers(Citrine::_PlatformContext* context)
{
	SwapBuffers(context->device);
}

bool _ContextGetEvent(Citrine::_PlatformContext* context, Citrine::PlatformEvent* event)
{
	//TODO: Implement this
	return false;
}
#endif
