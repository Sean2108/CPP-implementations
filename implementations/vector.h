#pragma once

#include <initializer_list>
#include <mutex>

namespace implementations {

	template <class T>
	class Vector {
		size_t m_size;
		size_t m_capacity;
		T* m_data;
		std::recursive_mutex m_mutex;

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
		T* find(const T& value) const;

		// modifiers
		void clear() noexcept;
		void reserve(const size_t capacity);
		void push_back(const T& newObj);
		void push_back(T&& newObj);
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

	template<>
	class Vector<bool> {
		size_t m_size;
		size_t m_charSize;
		size_t m_capacity;
		char* m_data;
		std::recursive_mutex m_mutex;

	public:
		// constructors
		Vector<bool>();
		Vector<bool>(const size_t size, const bool value = false);
		Vector<bool>(std::initializer_list<bool>&& values);
		Vector<bool>(const bool* const begin, const bool* const end);
		Vector<bool>(const Vector& otherVec);
		Vector<bool>(Vector&& otherVec) noexcept;
		Vector<bool>& operator=(const Vector& otherVec);
		Vector<bool>& operator=(Vector&& otherVec) noexcept;
		~Vector<bool>();

		// getters
		size_t size() const noexcept;
		size_t capacity() const noexcept;
		bool empty() const noexcept;
		bool at(const size_t index);
		bool operator[](const size_t index);

		// modifiers
		void clear() noexcept;
		void reserve(const size_t capacity);
		void push_back(const bool value);
		void pop_back();

		// free functions
		friend bool operator==(const Vector<bool>& a, const Vector<bool>& b);
		friend bool operator!=(const Vector<bool>& a, const Vector<bool>& b);
		friend void swap(Vector<bool>& a, Vector<bool>& b) noexcept;
	};
}
