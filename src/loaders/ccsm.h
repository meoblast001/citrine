/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CCSM_H
#define CCSM_H

#include <stdio.h>
#include "../contexts/graphics/cgraphicscontext.h"

Citrine::MeshData* LoadCSM(FILE* file, Citrine::IGraphicsContext* graphics_context, const char* directory);
#endif
