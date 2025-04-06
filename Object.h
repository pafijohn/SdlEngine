#pragma once

class QuadTree;

#include <unordered_set>
#include <unordered_map>
#include <cstdint>

template <class T, int N>
class Pool
{
protected:
	static inline const int size = N;
	
	static inline T objects[N];
	
	static inline std::unordered_set<uint32_t> available;
	static inline std::unordered_set<uint32_t> taken;
	static inline std::unordered_map<T*, uint32_t> indexes;
	
public:
	static void Init()
	{
		for (int i = 0; i < Pool<T, N>::size; i++)
		{
			Pool<T, N>::available.insert(i);
		}
	}
	
	static T& GetObject()
	{
		int idx = *(Pool<T, N>::available.begin());
		
		Pool<T, N>::available.erase(idx);
		Pool<T, N>::taken.insert(idx);
		T& obj = Pool<T, N>::objects[idx];
		
		Pool<T, N>::indexes[&obj] = idx;
		
		return Pool<T, N>::objects[idx];
	}
	
	static void ReturnObject(T& obj)
	{
		int idx = Pool<T, N>::indexes[&obj];
		
		Pool<T, N>::taken.erase(idx);
		Pool<T, N>::available.insert(idx);
	}
};

template <class T, int N>
class PoolObject
{
	T* obj;
public:
	PoolObject()
	{
		this->obj = nullptr;
	}
	
	~PoolObject()
	{
	}
	
	T& Take()
	{
		this->obj = &Pool<T, N>::GetObject();
		return *this->obj;
	}
	
    T* operator->() {
        return obj;
    }
	
	bool IsNull()
	{
		return this->obj == nullptr;
	}
	
	T& Get()
	{
		return *this->obj;
	}
};

typedef Pool<QuadTree, 1000> QuadTreePool;
typedef PoolObject<QuadTree, 1000> PooledQuadTree;
