#include "string.h"

#include <algorithm>
#include <cstring>

namespace implementations
{
	String::String() : String("")
	{
	}

	String::String(const char* otherString) : m_size{ strlen(otherString) }, m_data{ new char[m_size + 1] }
	{
		if (m_size > 0) {
			std::copy(otherString, otherString + m_size, m_data);
		}
		m_data[m_size] = '\0';
	}

	String::~String()
	{
		delete[] m_data;
	}

	String::String(const String& otherString): m_size{otherString.m_size}, m_data{new char[m_size + 1]}
	{
		std::copy(otherString.m_data, otherString.m_data + m_size + 1, m_data);
	}

	String::String(String&& otherString) noexcept : String{}
	{
		swap(*this, otherString);
	}

	String& String::operator=(String otherString) noexcept
	{
		swap(*this, otherString);
		return *this;
	}

	String& String::operator=(String&& otherString) noexcept
	{
		swap(*this, otherString);
		return *this;
	}

	int String::length() const
	{
		return m_size;
	}

	bool String::empty() const
	{
		return m_size == 0;
	}

	bool String::operator==(const String& otherString) const
	{
		return m_size == otherString.m_size && strcmp(m_data, otherString.m_data) == 0;
	}

	void swap(String& a, String& b) noexcept
	{
		std::swap(a.m_size, b.m_size);
		std::swap(a.m_data, b.m_data);
	}

	std::ostream& operator<<(std::ostream& stream, const String& s)
	{
		stream << s.m_data;
		return stream;
	}
} // namespace implementations

