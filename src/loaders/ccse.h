/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CCSE_H
#define CCSE_H

#include <stdio.h>
#include "../contexts/graphics/cgraphicscontext.h"
#include "centityloader.h"

Citrine::World* LoadCSE(FILE* file, Citrine::IGraphicsContext* graphics_context, Citrine::IEntityLoader* entity_loader, const char* directory);
#endif
