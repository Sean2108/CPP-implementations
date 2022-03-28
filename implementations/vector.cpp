#include "vector.h"

#include <algorithm>
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
		if (index >= m_size) {
			throw std::out_of_range("Index out of bounds");
		}
		return m_data[index];
	}

	template <class T>
	T& Vector<T>::operator[](const size_t index) {
		return m_data[index];
	}

	template <class T>
	void Vector<T>::clear() {
		delete[] m_data;
		m_data = nullptr;
		m_size = 0;
		m_capacity = 0;
	}

	template <class T>
	void Vector<T>::reserve(const size_t capacity) {
		if (capacity > m_capacity) {
			m_capacity = capacity;
			T* newData = new T[capacity];
			std::copy(m_data, m_data + m_size, newData);
			std::swap(m_data, newData);
			delete[] newData;
		}
	}

	template <class T>
	void Vector<T>::push_back(T newObj) {
		if (m_size == m_capacity) {
			reserve(m_capacity ? m_capacity * 2 : 1);
		}
		m_data[m_size++] = newObj;
	}

	template <class T>
	template <class ...Args>
	void Vector<T>::emplace_back(Args&&... args) {
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
}