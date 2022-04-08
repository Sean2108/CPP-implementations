#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>

namespace implementations {
	/*
	* implementation of a linked unordered map
	* similar to OrderedDict in Python or LinkedHashMap in Java
	* O(1) insert at the ends, O(1) remove, O(1) lookup
	*/
	template <class K, class V>
	class LinkedUnorderedMap
	{
		struct LinkedNode;
		using LinkedNodePtr = std::shared_ptr<LinkedNode>;

		struct LinkedNode {
			K key;
			V value;
			LinkedNodePtr next;
			LinkedNodePtr prev;

			LinkedNode(const K& key, const V& value, const LinkedNodePtr& next = nullptr, const LinkedNodePtr& prev = nullptr);
		};

		std::unordered_map<K, LinkedNodePtr> m_nodesMap;
		LinkedNodePtr m_head;
		LinkedNodePtr m_tail;
		size_t m_length;
		mutable std::recursive_mutex m_mutex;
	public:
		LinkedUnorderedMap();

		bool hasKey(const K& key) const;
		V& operator[](const K& key) const;
		size_t size() const noexcept;
		bool empty() const noexcept;

		bool insertAtHead(const K& key, const V& value);
		bool insertAtTail(const K& key, const V& value);

		V remove(const K& key);
		std::pair<K, V> remove(const bool removeFirstItem);

		bool moveToEnd(const K& key);
	};
}

