#include "lru_cache.h"

#include "linked_unordered_map.h"

namespace implementations {
	template <class K, class V>
	LruCache<K, V>::LruCache(const size_t capacity)
		: m_map(), m_capacity(capacity) {}

	template<class K, class V>
	size_t LruCache<K, V>::size() const
	{
		return m_map.size();
	}

	template<class K, class V>
	bool LruCache<K, V>::empty() const
	{
		return size() == 0;
	}

	template<class K, class V>
	bool LruCache<K, V>::hasKey(const K& key) const
	{
		return m_map.hasKey(key);
	}

	template<class K, class V>
	V& LruCache<K, V>::get(const K& key)
	{
		if (m_map.moveToEnd(key)) {
			return m_map[key];
		}
		throw std::invalid_argument(key + " does not exist in map");
	}

	template<class K, class V>
	V& LruCache<K, V>::operator[](const K& key)
	{
		return get(key);
	}

	template<class K, class V>
	void LruCache<K, V>::push(const K& key, const V& value)
	{
		if (hasKey(key)) {
			m_map[key] = value;
			m_map.moveToEnd(key);
			return;
		}
		m_map.insertAtTail(key, value);
		if (size() > m_capacity) {
			m_map.remove(true);
		}
	}


}
