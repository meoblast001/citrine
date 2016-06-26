/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CCANVAS_H
#define CCANVAS_H

#include "csprite2d.h"
#include "../csmartpointer.h"

namespace Citrine
{
	class Canvas
	{
		public:
			/*Sprite2Ds*/
			std::list<SmartPointer<Sprite2D> >* GetSprite2Ds();
			SmartPointer<Sprite2D> CreateSprite2D();
			void AddSprite2D(SmartPointer<Sprite2D> sprite2d);
			void RemoveSprite2D(SmartPointer<Sprite2D> sprite2d);
		private:
			std::list<SmartPointer<Sprite2D> > sprite2ds;
	};
}
#endif
