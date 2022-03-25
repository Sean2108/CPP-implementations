#ifndef IMPLEMENTATIONS_STRING
#define IMPLEMENTATIONS_STRING

namespace implementations
{
	class String
	{
		unsigned int m_size;
		const char m_data[];

	public:
		String();
		~String();
		String(const String& otherString);
		String(String&& otherString) noexcept;
		String& operator=(String otherString);
		String& operator=(String&& otherString) noexcept;

	};
} // namespace implementations

#endif