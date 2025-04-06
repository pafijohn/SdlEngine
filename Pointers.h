#pragma once

#include "Utils.h"

namespace Pointers
{
	template <class T>
	class SmartPointer
	{
		T* internal = nullptr;
		int* refCount = nullptr;
		
	public:
		SmartPointer()
		{
		}
		
		SmartPointer(T* elem)
		{
			this->Set(elem);
		}
		
		SmartPointer(const SmartPointer<T>& other)
		{
			this->Set(other);
		}
		
		void Set(const SmartPointer<T>& other)
		{
			this->Clear();
			this->internal = other.internal;
			this->refCount = other.refCount;
			
			(*this->refCount)++;
		}
		
		void Set(T* elem)
		{
			this->Clear();
			this->internal = elem;
			this->refCount = new int;
			(*this->refCount) = 1;
		}
		
		~SmartPointer()
		{
			this->Clear();
		}
		
		bool IsNull() const
		{
			return (this->internal == nullptr);
		}
		
		operator bool() const
		{
			return !this->IsNull();
		}
		
		T* operator->()
		{
			return this->Get();
		}
		
		T* operator*()
		{
			return this->Get();
		}
		
		SmartPointer<T>& operator=(T* elem)
		{
			this->Set(elem);
			return *this;
		}
		
		SmartPointer<T>& operator=(const SmartPointer<T>& elem)
		{
			this->Set(elem);
			return *this;
		}
		
		T* Get()
		{
			return this->internal;
		}
		
		T& GetRef()
		{
			return (*this->internal);
		}
		
		void Clear()
		{
			if (this->internal && this->refCount)
			{
				(*this->refCount)--;
				
				if ((*this->refCount) == 0)
				{
					delete this->internal;
					delete this->refCount;
					this->internal = nullptr;
					this->refCount = nullptr;
				}
			}
		}
	};
	
	template <class T>
	using SP = SmartPointer<T>;
	
	template <class T, class ... Args>
	SP<T> create(Args ... args)
	{
		return SP<T>(new T(args...));
	}
}