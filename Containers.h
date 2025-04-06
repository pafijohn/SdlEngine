#pragma once

#include <cstring>
#include <cstdint>

#include "Utils.h"

namespace Containers
{
	template <class T>
	class Vector
	{
	protected:
		size_t size;
		T** internal = nullptr;
	public:
		Vector()
		{
			this->size = 0;
			this->internal = nullptr;
		}
		
		virtual ~Vector()
		{
			this->Clear();
		}
		
		size_t Size()
		{
			return this->size;
		}
		
		void Swap(const T& first, const T& second)
		{
			int fi = this->IndexOf(first);
			int si = this->IndexOf(second);
			
			if (fi >= 0 && si >= 0)
			{
				this->Swap(fi, si);
			}
		}
		
		void Swap(int first, int second)
		{
			if (this->IsValidIndex(first) && this->IsValidIndex(second))
			{
				T* temp = this->internal[first];
				this->internal[first] = this->internal[second];
				this->internal[second] = temp;
			}
		}
		
		void Clear()
		{
			if (this->internal)
			{
				delete this->internal;
				this->internal = nullptr;
			}
			
			this->size = 0;
		}
		
		void PushAt(const size_t& index, const T& elem)
		{
			if (this->IsValidIndex(index))
			{
				T* newElem = new T(elem);
				size_t before = index;
				size_t after = (this->size - index);
				this->size++;
				
				T** temp = new T*[this->size];
				
				if (before > 0)
				{
					memcpy(temp, this->internal, before * sizeof(T*));
				}
				
				temp[index] = newElem;
				
				if (after > 0)
				{
					memcpy(&temp[index + 1], &this->internal[index], after * sizeof(T*));
				}
				
				this->internal = temp;
			}
		}
		
		int IndexOf(const T& elem)
		{
			for (size_t i = 0; i < this->size; i++)
			{
				if (elem == *this->internal[i])
				{
					return (int)i;
				}
			}
			
			return -1;
		}
		
		
		void PushBack(const T& elem)
		{
			T* newElem = new T(elem);
			
			size_t oldSize = this->size;
			this->size++;
			
			T** temp = new T*[this->size];
			
			if (oldSize > 0)
			{
				for (size_t i = 0; i < oldSize; i++)
				{
					temp[i] = this->internal[i];
				}
			}
			
			temp[oldSize] = newElem;
			
			this->internal = temp;
		}
		
		void PushFront(const T& elem)
		{
			T* newElem = new T(elem);
			
			size_t oldSize = this->size;
			this->size++;
			
			T** temp = new T*[this->size];
			
			if (oldSize > 0)
			{
				memcpy(&temp[1], this->internal, sizeof(T**) * oldSize);
			}
			
			temp[0] = newElem;
			
			this->internal = temp;
		}
		
		bool IsValidIndex(const size_t& index)
		{
			return isBounded(index, 0, this->size);
		}
		
		T& At(const size_t& index)
		{
			// We may crash if an incorrect index is given
			// Could construct a new object and return that
			// but that may be confusing
			//
			// So I'm passing the buck of checking if its
			// safe to the caller, a call to IsValidIndex
			// should be sufficient
			return *this->internal[index];
		}
		
		T* Set(const size_t& index, const T& elem)
		{
			T* removing = this->internal[index];
			
			T* newElem = new T(elem);
			this->internal[index] = newElem;
			
			return removing;
		}
		
		void Pop(const size_t& index)
		{
			if (this->IsValidIndex(index))
			{
				size_t before = index;
				size_t after = (this->size - index - 1);
				T* elem = this->internal[index];
				this->size--;
				
				T** temp = new T*[this->size];
				
				if (before > 0)
				{
					memcpy(temp, this->internal, before * sizeof(T*));
				}
				
				if (after > 0)
				{
					memcpy(&temp[index], &this->internal[index + 1], after * sizeof(T*));
				}
				
				delete elem;
				this->internal = temp;
			}
		}
	};

}