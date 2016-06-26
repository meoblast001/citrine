/*
Copyright (C) 2010 Braden Walters

This software may be modified and distributed under the terms of the MIT
license. See the LICENSE file for details.
*/

#ifndef CSMARTPOINTER_H
#define CSMARTPOINTER_H

namespace Citrine
{
	//For internal use only; See class SmartPointer
	class TypelessSmartPointer
	{
		public:
			TypelessSmartPointer();
			void* Destroy();
			void* GetPtr() const;
			void* Set(const TypelessSmartPointer& other);
			void* Set(void* other);
		protected:
			struct SmartPointerProperties
			{
				void* ptr;
				unsigned int refcount;
			};

			SmartPointerProperties* GetProperties() const;
		private:
			SmartPointerProperties* properties;
	};

	template<class T>
	class SmartPointer
	{
		public:
			inline SmartPointer()
			{
			}

			inline SmartPointer(const SmartPointer<T>& other)
			{
				typeless_smartptr.Set(other.GetTypelessSmartPointer());
			}

			inline SmartPointer(T* other)
			{
				typeless_smartptr.Set(other);
			}

			inline ~SmartPointer()
			{
				T* result = (T*) typeless_smartptr.Destroy();
				if (result)
					delete result;
			}

			inline SmartPointer<T> operator=(const SmartPointer<T>& other)
			{
				T* result = (T*) typeless_smartptr.Set(other.GetTypelessSmartPointer());
				if (result)
					delete result;
			}

			inline SmartPointer<T> operator=(T* other)
			{
				T* result = (T*) typeless_smartptr.Set(other);
				if (result)
					delete result;
			}

			inline T* operator->() const
			{
				return (T*) typeless_smartptr.GetPtr();
			}

			inline T* operator*() const
			{
				return (T*) typeless_smartptr.GetPtr();
			}
		protected:
			const TypelessSmartPointer& GetTypelessSmartPointer() const
			{
				return typeless_smartptr;
			}
		private:
			TypelessSmartPointer typeless_smartptr;
	};
}
#endif
