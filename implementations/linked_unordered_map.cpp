#include "linked_unordered_map.h"

#include <stdexcept>

namespace implementations {
	template <class K, class V>
	LinkedUnorderedMap<K, V>::LinkedNode::LinkedNode(const K& key, const V& value, const LinkedNodePtr& next, const LinkedNodePtr& prev)
		: key(key), value(value), next(next), prev(prev) {}

	template <class K, class V>
	LinkedUnorderedMap<K, V>::LinkedUnorderedMap()
		: m_nodesMap()
		, m_head(std::make_shared<LinkedNode>(K(), V()))
		, m_tail(m_head)
		, m_length(0)
		, m_mutex() {}

	template<class K, class V>
	bool LinkedUnorderedMap<K, V>::hasKey(const K& key) const
	{
		return m_nodesMap.find(key) != m_nodesMap.cend();
	}

	template <class K, class V>
	V& LinkedUnorderedMap<K, V>::operator[](const K& key) const {
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		const auto it = m_nodesMap.find(key);
		if (it == m_nodesMap.cend()) {
			throw std::invalid_argument(key + " does not exist in map");
		}
		return it->second->value;
	}

	template <class K, class V>
	size_t LinkedUnorderedMap<K, V>::size() const noexcept {
		return m_length;
	}

	template <class K, class V>
	bool LinkedUnorderedMap<K, V>::empty() const noexcept {
		return m_length == 0;
	}

	template <class K, class V>
	bool LinkedUnorderedMap<K, V>::insertAtHead(const K& key, const V& value) {
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		if (!m_head->next) {
			return insertAtTail(key, value);
		}
		if (m_nodesMap.find(key) != m_nodesMap.cend()) {
			return false;
		}
		m_head->next = make_shared<LinkedNode>(key, value, m_head->next, m_head);
		m_length++;
		m_nodesMap[key] = m_head->next;
		return true;
	}

	template <class K, class V>
	bool LinkedUnorderedMap<K, V>::insertAtTail(const K& key, const V& value) {
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		if (m_nodesMap.find(key) != m_nodesMap.cend()) {
			return false;
		}
		m_tail->next = make_shared<LinkedNode>(key, value, nullptr, m_tail);
		m_tail = m_tail->next;
		m_length++;
		m_nodesMap[key] = m_tail;
		return true;
	}

	template <class K, class V>
	V LinkedUnorderedMap<K, V>::remove(const K& key) {
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		const auto it = m_nodesMap.find(key);
		if (it == m_nodesMap.cend()) {
			throw std::invalid_argument(key + " does not exist in map");
		}
		if (!it->second->next) {
			m_tail = it->second->prev;
			m_tail->next = nullptr;
		}
		else {
			it->second->prev->next = it->second->next;
			it->second->next->prev = it->second->prev;
		}
		V value = it->second->value;
		m_nodesMap.erase(it);
		m_length--;
		return value;
	}

	template <class K, class V>
	std::pair<K, V> LinkedUnorderedMap<K, V>::remove(const bool removeFirstItem) {
		if (empty()) {
			throw std::runtime_error("Cannot remove from empty map");
		}
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		auto toBeRemoved = removeFirstItem ? m_head->next : m_tail;
		remove(toBeRemoved->key);
		return std::make_pair(toBeRemoved->key, toBeRemoved->value);
	}

	template <class K, class V>
	bool LinkedUnorderedMap<K, V>::moveToEnd(const K& key) {
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		return insertAtTail(key, remove(key));
	}
}