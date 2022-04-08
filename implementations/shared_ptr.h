#pragma once

#include <utility>

namespace implementations {

	template <class T>
	class SharedPtr {
		T* m_underlyingPtr;
		size_t* m_refCount;
	public:
		// constructors and assignment operators
		SharedPtr(T* ptr = nullptr);
		SharedPtr(const SharedPtr<T>& ptr);
		SharedPtr(SharedPtr<T>&& ptr) noexcept;
		SharedPtr& operator=(const SharedPtr<T>& ptr);
		SharedPtr& operator=(SharedPtr<T>&& ptr) noexcept;
		~SharedPtr();

		// getters
		T* get() const noexcept;
		T* operator->() noexcept;
		T& operator*() noexcept;
		operator bool() const noexcept;

		size_t useCount() noexcept;
		void reset() noexcept;

		// free functions
		template <class Type>
		friend void swap(SharedPtr<Type>& a, SharedPtr<Type>& b) noexcept;
	};

	template <class T, class ...Args>
	SharedPtr<T> makeShared(Args&&... args);
}

