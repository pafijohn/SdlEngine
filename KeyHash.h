#pragma once

#include "HashMapShared.h"

class Hashable
{
public:
	virtual HashType Hash() const = 0;
};

// Default hash function class
template <class K>
class KeyHash
{
public:
    HashType operator()(const K& key) const
    {
        return static_cast<HashType>(key);
    }
};

template <>
class KeyHash<Hashable>
{
public:
    HashType operator()(const Hashable& key) const
    {
        return key.Hash();
    }
};

