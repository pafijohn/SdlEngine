#pragma once

#include "KeyHash.h"
#include "HashMapShared.h"

// Linked list pair
template <typename K, typename V>
class HashNode
{
public:
	HashNode(const K& key, const V& value):
		key(key),
		value(value),
		next(nullptr)
	{
	}

	K& GetKey()
	{
		return this->key;
	}

	V& GetValue()
	{
		return this->value;
	}

	void SetValue(const V& value)
	{
		this->value = value;
	}

	HashNode* GetNext() const
	{
		return this->next;
	}

	void SetNext(HashNode *next)
	{
		this->next = next;
	}

private:
	K key;
	V value;
	HashNode* next;
};

template <typename K, typename V, typename F = KeyHash<K>>
class HashMap
{
protected:
	HashType GetHash(const K& key)
	{
		HashType hashValue = this->hashFunc(key);
		return hashValue % HASH_MAP_TABLE_SIZE;
	}
public:
	HashMap()
	{
		// construct zero initialized hash table of size
		this->table = new HashNode<K, V>*[HASH_MAP_TABLE_SIZE]();
	}

	~HashMap()
	{
		// destroy all buckets one by one
		for (int i = 0; i < HASH_MAP_TABLE_SIZE; ++i)
		{
			HashNode<K, V>* entry = table[i];
			while (entry != nullptr)
			{
				HashNode<K, V>* prev = entry;
				entry = entry->GetNext();
				delete prev;
			}
			this->table[i] = nullptr;
		}
		// destroy the hash table
		delete [] this->table;
	}
	
	bool Get(const K& key, V& value)
	{
		HashType hashValue = this->GetHash(key);
		HashNode<K, V>* entry = this->table[hashValue];

		while (entry != nullptr)
		{
			if (entry->GetKey() == key)
			{
				value = entry->GetValue();
				return true;
			}
			entry = entry->GetNext();
		}
		return false;
	}

	void Put(const K& key, const V& value)
	{
		HashType hashValue = this->GetHash(key);
		HashNode<K, V>* prev = nullptr;
		HashNode<K, V>* entry = this->table[hashValue];

		while (entry != nullptr && entry->GetKey() != key)
		{
			prev = entry;
			entry = entry->GetNext();
		}

		if (entry == nullptr)
		{
			entry = new HashNode<K, V>(key, value);
			if (prev == nullptr)
			{
				// insert as first bucket
				this->table[hashValue] = entry;
			}
			else
			{
				prev->SetNext(entry);
			}
		}
		else
		{
			// just update the value
			entry->SetValue(value);
		}
	}

	void Remove(const K& key)
	{
		HashType hashValue = this->GetHash(key);
		HashNode<K, V>* prev = nullptr;
		HashNode<K, V>* entry = this->table[hashValue];

		while (entry != nullptr && entry->GetKey() != key)
		{
			prev = entry;
			entry = entry->GetNext();
		}

		if (entry == nullptr)
		{
			return;
		}
		else
		{
			if (prev == nullptr)
			{
				this->table[hashValue] = entry->GetNext();
			}
			else
			{
				prev->SetNext(entry->GetNext());
			}
			delete entry;
		}
	}

protected:
	HashNode<K, V>** table;
	F hashFunc;
};
