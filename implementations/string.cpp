#include "string.h"

#include <algorithm>
#include <cstring>
#include <stack>

namespace implementations {
	String::String() : String("") {
	}

	String::String(const char otherChar)
		: m_size{ 1 }
		, m_capacity{ m_size + 1 }
		, m_data{ new char[2] } {
		m_data[0] = otherChar;
		m_data[1] = '\0';
	}

	String::String(const char* otherString)
		: m_size{ strlen(otherString) }
		, m_capacity{ m_size + 1 }
		, m_data{ new char[m_size + 1] } {
		if (m_size > 0) {
			std::copy(otherString, otherString + m_size, m_data);
		}
		m_data[m_size] = '\0';
	}

	String::~String() {
		delete[] m_data;
	}

	String::String(const String& otherString)
		: m_size{ otherString.m_size }
		, m_capacity{ m_size + 1 }
		, m_data{ new char[m_size + 1] } {
		std::copy(otherString.m_data, otherString.m_data + m_size + 1, m_data);
	}

	String::String(String&& otherString) noexcept : String{} {
		swap(*this, otherString);
	}

	String& String::operator=(const String& otherString) {
		String temp{ otherString };
		swap(*this, temp);
		return *this;
	}

	String& String::operator=(String&& otherString) noexcept {
		swap(*this, otherString);
		return *this;
	}

	String& String::operator+=(const String& otherString) {
		*this = *this + otherString;
		return *this;
	}

	size_t String::length() const {
		return m_size;
	}

	size_t String::capacity() const {
		return m_capacity;
	}

	bool String::empty() const {
		return m_size == 0;
	}

	int String::find(const String& otherString) const {
		std::lock_guard<std::mutex> lock(m_mutex);
		char* substring = strstr(m_data, otherString.m_data);
		if (!substring) {
			return -1;
		}
		return m_size - strlen(substring);
	}

	char& String::operator[](const size_t i) const {
		std::lock_guard<std::mutex> lock(m_mutex);
		if (i >= m_size) {
			throw std::runtime_error("Index out of bounds");
		}
		return m_data[i];
	}

	void String::push_back(const char c) {
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_size + 1 == m_capacity) {
			reserve(m_capacity * 2);
		}
		m_data[m_size++] = c;
		m_data[m_size] = '\0';
	}

	void String::pop_back() {
		m_data[--m_size] = '\0';
	}

	void String::reserve(const size_t newCapacity) {
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_capacity < newCapacity) {
			m_capacity = newCapacity;
			char* newString = new char[m_capacity];
			std::copy(m_data, m_data + m_size, newString);
			std::swap(newString, m_data);
			delete[] newString;
			m_data[m_size] = '\0';
		}
	}

	char* String::begin() {
		return m_data;
	}

	char* String::end() {
		return m_data + m_size;
	}

	const char* String::cbegin() const {
		return m_data;
	}

	const char* String::cend() const {
		return m_data + m_size;
	}

	int stoi(const String& string) {
		int outputNumber = 0;
		if (string.empty()) {
			throw std::invalid_argument("String is empty");
		}
		const bool isNegative = string.m_data[0] == '-';
		const size_t startIndex = isNegative ? 1 : 0;
		if (isNegative && string.length() == 1) {
			throw std::invalid_argument("String does not have any characters after negative sign");
		}
		for (size_t i = startIndex; i < string.m_size; i++) {
			if (string.m_data[i] < '0' || string.m_data[i] > '9') {
				throw std::invalid_argument("String contains non-numerical characters");
			}
			const size_t digit = string.m_data[i] - '0';
			if (outputNumber * 10 < outputNumber) {
				throw std::runtime_error("Integer overflow");
			}
			outputNumber = outputNumber * 10 + digit;
		}
		return isNegative ? -1 * outputNumber : outputNumber;
	}

	String toString(int number) {
		if (number == 0) {
			return String("0");
		}
		const bool isNegative = number < 0;
		number = std::abs(number);
		std::stack<char> characters;
		while (number > 0) {
			characters.push('0' + number % 10);
			number /= 10;
		}
		const size_t outputStringLength = characters.size() + 1 + (isNegative ? 1 : 0);
		char* newString = new char[outputStringLength];
		size_t index = 0;
		if (isNegative) {
			newString[index++] = '-';
		}
		while (!characters.empty()) {
			newString[index++] = characters.top();
			characters.pop();
		}
		newString[outputStringLength - 1] = '\0';
		String newStringObj = String(newString);
		delete[] newString;
		return newStringObj;
	}

	bool operator==(const String& a, const String& b) {
		return a.m_size == b.m_size && strcmp(a.m_data, b.m_data) == 0;
	}

	bool operator!=(const String& a, const String& b) {
		return !(a == b);
	}

	void swap(String& a, String& b) noexcept {
		std::swap(a.m_size, b.m_size);
		std::swap(a.m_capacity, b.m_capacity);
		std::swap(a.m_data, b.m_data);
	}

	std::ostream& operator<<(std::ostream& stream, const String& s) {
		stream << s.m_data;
		return stream;
	}

	String operator+(const String& a, const String& b) {
		size_t newLength = a.length() + b.length() + 1;
		char* newString = new char[newLength];
		std::copy(a.m_data, a.m_data + a.m_size, newString);
		std::copy(b.m_data, b.m_data + b.m_size, newString + a.m_size);
		newString[newLength - 1] = '\0';
		String newStringObj(newString);
		delete[] newString;
		return newStringObj;
	}
} // namespace implementations

