#pragma once

namespace implementations {
	template <class T>
	class SharedPtr
	{
		T* const m_underlyingPtr;
		int* const m_refCount;
	public:
		SharedPtr(T* ptr = nullptr);
		~SharedPtr();

		T* get() noexcept;
		T* operator->() noexcept;
		T& operator*() noexcept;
	};
}

