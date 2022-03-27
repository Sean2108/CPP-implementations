#pragma once

#include <ostream>

namespace implementations {
	class String {
		size_t m_size;
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

		int length() const;
		bool empty() const;
		int find(const String& otherString) const;

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