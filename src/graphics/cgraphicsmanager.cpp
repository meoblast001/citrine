/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include "cgraphicsmanager.h"

Citrine::GraphicsManager::GraphicManager()
{
	if (already_initialized)
		Error("Citrine::GraphicsManager initialized more than once");
	already_initialized = true;
}

bool Citrine::GraphicsManager::already_initialized = false;
