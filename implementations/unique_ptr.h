#pragma once

namespace implementations {
	template <class T>
	class UniquePtr
	{
		T* m_underlyingPtr;
	public:
		// constructors
		UniquePtr(T* ptr = nullptr);
		UniquePtr(UniquePtr&& ptr) noexcept;
		UniquePtr& operator=(UniquePtr&& ptr) noexcept;
		~UniquePtr();

		// getters
		T* get() const noexcept;
		T* operator->() const noexcept;
		T& operator*() const noexcept;
		operator bool() const noexcept;

		// modifiers
		void reset() noexcept;

		// free functions
		template <class Type>
		void friend swap(UniquePtr<Type>& ptr1, UniquePtr<Type>& ptr2) noexcept;
	};
	template <class T, class... Args>
	UniquePtr<T> makeUnique(Args&&...);
}

