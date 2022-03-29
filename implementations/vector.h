#pragma once

#include <initializer_list>

namespace implementations {

	template <class T>
	class Vector {
		size_t m_size;
		size_t m_capacity;
		T* m_data;

	public:
		// constructors and assignment operators
		Vector();
		Vector(const size_t size, const T& value);
		Vector(std::initializer_list<T>&& list);
		Vector(const T* const begin, const T* const end);
		Vector(const Vector& otherVector);
		Vector(Vector&& otherVector) noexcept;
		Vector& operator=(const Vector& otherVector);
		Vector& operator=(Vector&& otherVector) noexcept;
		~Vector();

		// getters
		size_t size() const noexcept;
		size_t capacity() const noexcept;
		bool empty() const noexcept;
		T& at(const size_t index);
		T& operator[](const size_t index);

		// modifiers
		void clear();
		void reserve(const size_t capacity);
		void push_back(T newObj);
		template <class ...Args>
		void emplace_back(Args&&... args);
		void pop_back();
		T* erase(T* position);
		T* insert(T* position, const T& value);

		// iterators
		T* begin();
		T* end();
		const T* cbegin() const;
		const T* cend() const;

		// free functions
		template <class Type>
		friend bool operator==(const Vector<Type>& a, const Vector<Type>& b);
		template <class Type>
		friend bool operator!=(const Vector<Type>& a, const Vector<Type>& b);
		template <class Type>
		friend void swap(Vector<Type>& a, Vector<Type>& b);
	};
}
