#pragma once

#include <ostream>

namespace implementations {
	class String {
		size_t m_size;
		size_t m_capacity;
		char* m_data;

	public:
		// constructors
		String();
		String(const char otherString);
		String(const char* otherString);
		String(const String& otherString);
		String(String&& otherString) noexcept;
		~String();

		// assignment operators
		String& operator=(const String& otherString);
		String& operator=(String&& otherString) noexcept;
		String& operator+=(const String& otherString);

		size_t length() const;
		size_t capacity() const;
		bool empty() const;
		int find(const String& otherString) const;
		char& operator[](const size_t i) const;

		// modifiers
		void push_back(const char c);
		void pop_back();
		void reserve(const size_t capacity);

		// interators
		char* begin();
		char* end();
		char* cbegin() const;
		char* cend() const;

		// binary operators
		friend std::ostream& operator<<(std::ostream& stream, const String& s);
		friend String operator+(const String& a, const String& b);
		friend bool operator==(const String& a, const String& b);
		friend bool operator!=(const String& a, const String& b);

		// free functions
		friend void swap(String& a, String& b) noexcept;
		friend int stoi(const String& string);
	};
	String toString(int number);
} // namespace implementations