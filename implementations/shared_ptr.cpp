#include "shared_ptr.h"

#include <utility>

namespace implementations {
	template <class T>
	SharedPtr<T>::SharedPtr(T* ptr)
		: m_underlyingPtr{ ptr }
		, m_refCount{ new size_t(ptr == nullptr ? 0 : 1) } {
	}

	template <class T>
	SharedPtr<T>::SharedPtr(const SharedPtr<T>& ptr)
		: m_underlyingPtr{ ptr.m_underlyingPtr }
		, m_refCount{ ptr.m_refCount } {
		(*m_refCount)++;
	}

	template <class T>
	SharedPtr<T>::SharedPtr(SharedPtr<T>&& ptr) noexcept
		: SharedPtr{} {
		swap(*this, ptr);
	}

	template <class T>
	SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& ptr) {
		auto copy = ptr;
		swap(*this, copy);
		return *this;
	}

	template <class T>
	SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& ptr) noexcept {
		swap(*this, ptr);
		ptr.reset();
		return *this;
	}

	template <class T>
	SharedPtr<T>::~SharedPtr() {
		reset();
	}

	template <class T>
	T* SharedPtr<T>::get() const noexcept {
		return m_underlyingPtr;
	}

	template <class T>
	T* SharedPtr<T>::operator->() noexcept {
		return m_underlyingPtr;
	}

	template <class T>
	T& SharedPtr<T>::operator*() noexcept {
		return *m_underlyingPtr;
	}

	template <class T>
	SharedPtr<T>::operator bool() const noexcept {
		return m_underlyingPtr != nullptr;
	}

	template <class T>
	size_t SharedPtr<T>::useCount() noexcept {
		return *m_refCount;
	}

	template <class T>
	void SharedPtr<T>::reset() noexcept {
		(*m_refCount)--;
		if (*m_refCount <= 0) {
			delete m_refCount;
			delete m_underlyingPtr;
		}
		m_underlyingPtr = nullptr;
	}

	template <class T, class ...Args>
	SharedPtr<T> makeShared(Args&&... args) {
		return { new T(std::forward<Args>(args)...) };
	}

	template <class T>
	void swap(SharedPtr<T>& a, SharedPtr<T>& b) noexcept {
		std::swap(a.m_underlyingPtr, b.m_underlyingPtr);
		std::swap(a.m_refCount, b.m_refCount);
	}
}
