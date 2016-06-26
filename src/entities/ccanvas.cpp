/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/
#include "ccanvas.h"

using namespace Citrine;

std::list<SmartPointer<Sprite2D> >* Citrine::Canvas::GetSprite2Ds()
{
	return &sprite2ds;
}

SmartPointer<Sprite2D> Citrine::Canvas::CreateSprite2D()
{
	Sprite2D* sprite2d = new Sprite2D;
	SmartPointer<Sprite2D> smartsprite2d = sprite2d;
	AddSprite2D(smartsprite2d);
	return smartsprite2d;
}

void Citrine::Canvas::AddSprite2D(SmartPointer<Sprite2D> sprite2d)
{
	sprite2ds.push_back(sprite2d);
}

void Citrine::Canvas::RemoveSprite2D(SmartPointer<Sprite2D> sprite2d)
{
	for (std::list<SmartPointer<Sprite2D> >::iterator i = sprite2ds.begin(); i != sprite2ds.end(); i++)
	{
		if (**i == *sprite2d)
		{
			sprite2ds.erase(i);
			return;
		}
	}
}
