#ifndef IMPLEMENTATIONS_STRING
#define IMPLEMENTATIONS_STRING

#include <ostream>

namespace implementations
{
	class String
	{
		size_t m_size;
		char* m_data;

	public:
		String();
		String(const char* otherString);
		~String();
		String(const String& otherString);
		String(String&& otherString) noexcept;
		String& operator=(String otherString) noexcept;
		String& operator=(String&& otherString) noexcept;
		int length() const;
		bool empty() const;
		bool operator==(const String& otherString) const;
		friend void swap(String& a, String& b) noexcept;
		friend std::ostream& operator<<(std::ostream& stream, const String& s);
	};
} // namespace implementations

#endif