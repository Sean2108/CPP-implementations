#include "string.h"

namespace implementations
{
	String::String() : m_size{ 0 }, m_data{}
	{
	}

	String::~String()
	{
	}

	String::String(const String& otherString) : String()
	{
	}

	String::String(String&& otherString) noexcept : String()
	{
	}

	String& String::operator=(String otherString)
	{
		return *this;
	}

	String& String::operator=(String&& otherString) noexcept
	{
		return *this;
	}
} // namespace implementations

