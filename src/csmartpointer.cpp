/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#include "csmartpointer.h"

Citrine::TypelessSmartPointer::TypelessSmartPointer()
{
	properties = 0;
}

void* Citrine::TypelessSmartPointer::Destroy()
{
	if (properties)
	{
		//If properties exist, decrement the reference count
		properties->refcount--;
		if (properties->refcount == 0)
		{
			//If the reference count is equal to zero, delete properties and return pointer for deletion
			void* ptr = properties->ptr;
			delete properties;
			return ptr;
		}
	}
	//Return NULL if the pointer is not to be deleted
	return 0;
}

void* Citrine::TypelessSmartPointer::GetPtr() const
{
	return properties->ptr;
}

void* Citrine::TypelessSmartPointer::Set(const TypelessSmartPointer& other)
{
	//Destroy existing smart pointer (if present)
	void* result = Destroy();
	//Obtain properties and increment reference count
	properties = other.GetProperties();
	properties->refcount++;
	//Return result of Destroy function so it can be deleted if necessary
	return result;
}

void* Citrine::TypelessSmartPointer::Set(void* other)
{
	//Destroy existing smart pointer (if present)
	void* result = Destroy();
	//Create new smart pointer, with 1 reference (this)
	properties = new SmartPointerProperties;
	properties->ptr = other;
	properties->refcount = 1;
	//Return result of Destroy function so it can be deleted if necessary
	return result;
}

Citrine::TypelessSmartPointer::SmartPointerProperties* Citrine::TypelessSmartPointer::GetProperties() const
{
	return properties;
}
