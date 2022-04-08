#pragma once

#include "linked_unordered_map.h"

namespace implementations {
	template <class K, class V>
	class LruCache
	{
		LinkedUnorderedMap<K, V> m_map;
		size_t m_capacity;
	public:
		LruCache(const size_t capacity);

		size_t size() const;
		bool empty() const;
		bool hasKey(const K& key) const;
		V& get(const K& key);
		V& operator[](const K& key);

		void push(const K& key, const V& value);
	};
}

