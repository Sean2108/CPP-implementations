#include "vector.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace implementations {
	template <class T>
	Vector<T>::Vector()
		: m_size{ 0 }
		, m_capacity{ 0 }
		, m_data{ nullptr } {}

	template <class T>
	Vector<T>::Vector(const size_t size, const T& value)
		: m_size{ size }
		, m_capacity{ size }
		, m_data{ new T[size] } {
		std::fill(m_data, m_data + m_size, value);
	}

	template <class T>
	Vector<T>::Vector(std::initializer_list<T>&& list)
		: m_size{ list.size() }
		, m_capacity{ m_size }
		, m_data{ new T[m_size] } {
		std::copy(list.begin(), list.end(), m_data);
	}

	template <class T>
	Vector<T>::Vector(const T* const begin, const T* const end)
		: m_size(end - begin)
		, m_capacity{ m_size }
		, m_data{ new T[m_size] } {
		std::copy(begin, end, m_data);
	}

	template <class T>
	Vector<T>::Vector(const Vector& otherVector)
		: m_size{ otherVector.m_size }
		, m_capacity{ otherVector.m_capacity }
		, m_data{ new T[m_size] } {
		std::copy(otherVector.m_data, otherVector.m_data + otherVector.m_size, m_data);
	}

	template <class T>
	Vector<T>::Vector(Vector&& otherVector) noexcept
		: Vector{} {
		swap(*this, otherVector);
	}

	template <class T>
	Vector<T>& Vector<T>::operator=(const Vector& otherVector) {
		auto copy{ otherVector };
		swap(*this, copy);
		return *this;
	}

	template <class T>
	Vector<T>& Vector<T>::operator=(Vector&& otherVector) noexcept {
		swap(*this, otherVector);
		otherVector.clear();
		return *this;
	}

	template <class T>
	Vector<T>::~Vector() {
		clear();
	}

	template <class T>
	size_t Vector<T>::size() const noexcept {
		return m_size;
	}

	template <class T>
	size_t Vector<T>::capacity() const noexcept {
		return m_capacity;
	}

	template <class T>
	bool Vector<T>::empty() const noexcept {
		return m_size == 0;
	}

	template <class T>
	T& Vector<T>::at(const size_t index) {
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		if (index >= m_size) {
			throw std::out_of_range("Index out of bounds");
		}
		return m_data[index];
	}

	template <class T>
	T& Vector<T>::operator[](const size_t index) {
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		return m_data[index];
	}

	template <class T>
	T* Vector<T>::find(const T& value) const {
		return std::find(m_data, m_data + m_size, value);
	}

	template <class T>
	void Vector<T>::clear() noexcept {
		delete[] m_data;
		m_data = nullptr;
		m_size = 0;
		m_capacity = 0;
	}

	template <class T>
	void Vector<T>::reserve(const size_t capacity) {
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		if (capacity > m_capacity) {
			m_capacity = capacity;
			T* newData = new T[capacity];
			std::copy(m_data, m_data + m_size, newData);
			std::swap(m_data, newData);
			delete[] newData;
		}
	}

	template <class T>
	void Vector<T>::push_back(const T& newObj) {
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		if (m_size == m_capacity) {
			reserve(m_capacity ? m_capacity * 2 : 1);
		}
		m_data[m_size++] = newObj;
	}

	template <class T>
	void Vector<T>::push_back(T&& newObj) {
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		if (m_size == m_capacity) {
			reserve(m_capacity ? m_capacity * 2 : 1);
		}
		m_data[m_size++] = std::move(newObj);
	}

	template <class T>
	template <class ...Args>
	void Vector<T>::emplace_back(Args&&... args) {
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		if (m_size == m_capacity) {
			reserve(m_capacity ? m_capacity * 2 : 1);
		}
		m_data[m_size++] = T{ std::forward<Args>(args)... };
	}

	template <class T>
	void Vector<T>::pop_back() {
		m_size--;
	}

	template <class T>
	T* Vector<T>::erase(T* position) {
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		const size_t index = position - m_data;
		if (index >= m_size || index < 0) {
			return end();
		}
		std::copy(position + 1, end(), position);
		m_size--;
		return position;
	}

	template <class T>
	T* Vector<T>::insert(T* position, const T& value) {
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		if (m_size == m_capacity) {
			m_capacity *= 2;
		}
		T* temp = new T[m_capacity];
		T* endIt = std::copy(m_data, position, temp);
		*endIt = value;
		std::copy(position, m_data + m_size, endIt + 1);
		std::swap(m_data, temp);
		delete[] temp;
		m_size++;
		return endIt;
	}

	template <class T>
	T* Vector<T>::begin() {
		return m_data;
	}

	template <class T>
	T* Vector<T>::end() {
		return m_data + m_size;
	}

	template <class T>
	const T* Vector<T>::cbegin() const {
		return m_data;
	}

	template <class T>
	const T* Vector<T>::cend() const {
		return m_data + m_size;
	}

	template <class T>
	bool operator==(const Vector<T>& a, const Vector<T>& b) {
		return a.m_size == b.m_size && std::equal(a.cbegin(), a.cend(), b.cbegin());
	}

	template <class T>
	bool operator!=(const Vector<T>& a, const Vector<T>& b) {
		return !(a == b);
	}

	template <class T>
	void swap(Vector<T>& a, Vector<T>& b) {
		std::swap(a.m_size, b.m_size);
		std::swap(a.m_capacity, b.m_capacity);
		std::swap(a.m_data, b.m_data);
	}

	Vector<bool>::Vector()
		: m_size(0)
		, m_charSize(0)
		, m_capacity(0)
		, m_data(nullptr)
		, m_mutex() {}

	Vector<bool>::Vector(const size_t size, const bool value)
		: m_size(size)
		, m_charSize(std::ceil(static_cast<float>(size) / 8))
		, m_capacity(m_charSize * 8)
		, m_data(new char[m_charSize])
		, m_mutex() {
		// 11111111 = 2^8 - 1 = 255, fill all ones if default value is true
		std::fill(m_data, m_data + m_charSize, value ? 255 : 0);
	}

	Vector<bool>::Vector(std::initializer_list<bool>&& values)
		: Vector(values.begin(), values.end()) {}

	Vector<bool>::Vector(const bool* const begin, const bool* const end)
		: Vector(end - begin) {
		size_t valueIndex = 0;
		for (const bool* it = begin; it < end; it++) {
			m_data[valueIndex / 8] <<= 1;
			m_data[valueIndex++ / 8] |= (*it ? 1 : 0);
		}
		// pad excess with 0s
		m_data[m_charSize - 1] <<= (8 - valueIndex % 8);
	}

	Vector<bool>::Vector(const Vector<bool>& otherVec)
		: Vector(otherVec.size()) {
		std::copy(otherVec.m_data, otherVec.m_data + otherVec.m_charSize, m_data);
	}

	Vector<bool>::Vector(Vector<bool>&& otherVec) noexcept : Vector() {
		swap(otherVec, *this);
	}

	Vector<bool>& Vector<bool>::operator=(const Vector<bool>& otherVec) {
		Vector<bool> copy = otherVec;
		swap(*this, copy);
		return *this;
	}

	Vector<bool>& Vector<bool>::operator=(Vector<bool>&& otherVec) noexcept {
		swap(*this, otherVec);
		otherVec.clear();
		return *this;
	}

	Vector<bool>::~Vector() {
		clear();
	}

	size_t Vector<bool>::size() const noexcept {
		return m_size;
	}

	size_t Vector<bool>::capacity() const noexcept {
		return m_capacity;
	}

	bool Vector<bool>::empty() const noexcept {
		return m_size == 0;
	}

	bool Vector<bool>::at(const size_t index) {
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		if (index >= m_size) {
			throw std::out_of_range("Index out of bounds");
		}
		return operator[](index);
	}

	bool Vector<bool>::operator[](const size_t index) {
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		const char bitmask = 1 << (7 - index % 8);
		return (m_data[index / 8] & bitmask) == bitmask;
	}

	void Vector<bool>::clear() noexcept {
		delete[] m_data;
		m_data = nullptr;
		m_size = 0;
		m_capacity = 0;
	}

	void Vector<bool>::reserve(const size_t capacity) {
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		if (capacity > m_capacity) {
			m_capacity = capacity;
			char* newData = new char[std::ceil(static_cast<float>(m_capacity) / 8)];
			std::copy(m_data, m_data + m_charSize, newData);
			std::swap(m_data, newData);
			delete[] newData;
		}
	}

	void Vector<bool>::push_back(const bool value) {
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		if (m_size == m_capacity) {
			reserve(m_capacity * 2);
		}
		if (m_size % 8 == 0) {
			m_charSize++;
		}
		if (value) {
			m_data[m_charSize - 1] |= 1 << (7 - m_size++ % 8);
		}
		else {
			m_data[m_charSize - 1] &= 255 ^ (1 << (7 - m_size++ % 8));
		}
	}

	void Vector<bool>::pop_back() {
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		if (--m_size % 8 == 0) {
			m_charSize--;
		}
	}

	bool operator==(const Vector<bool>& a, const Vector<bool>& b) {
		if (a.m_size != b.m_size) {
			return false;
		}
		if (!std::equal(a.m_data, a.m_data + a.m_charSize - 1, b.m_data)) {
			return false;
		}
		return a.m_data[a.m_charSize - 1] >> (a.m_charSize * 8 - a.m_size)
			== b.m_data[b.m_charSize - 1] >> (b.m_charSize * 8 - b.m_size);
	}

	bool operator!=(const Vector<bool>& a, const Vector<bool>& b) {
		return !(a == b);
	}

	void swap(Vector<bool>& a, Vector<bool>& b) noexcept {
		std::swap(a.m_size, b.m_size);
		std::swap(a.m_charSize, b.m_charSize);
		std::swap(a.m_capacity, b.m_capacity);
		std::swap(a.m_data, b.m_data);
	}
}
