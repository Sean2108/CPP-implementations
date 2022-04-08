#include "unique_ptr.h"

#include <utility>

namespace implementations {
	template <class T>
	UniquePtr<T>::UniquePtr(T* ptr) : m_underlyingPtr(ptr) {}

	template <class T>
	UniquePtr<T>::UniquePtr(UniquePtr<T>&& ptr) noexcept
		: m_underlyingPtr(nullptr) {
		swap(*this, ptr);
	}

	template <class T>
	UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr<T>&& ptr) noexcept {
		swap(*this, ptr);
		ptr.reset();
		return *this;
	}

	template <class T>
	UniquePtr<T>::~UniquePtr() {
		reset();
	}

	template <class T>
	T* UniquePtr<T>::get() const noexcept {
		return m_underlyingPtr;
	}

	template <class T>
	T* UniquePtr<T>::operator->() const noexcept {
		return m_underlyingPtr;
	}

	template <class T>
	T& UniquePtr<T>::operator*() const noexcept {
		return *m_underlyingPtr;
	}

	template <class T>
	UniquePtr<T>::operator bool() const noexcept {
		return m_underlyingPtr != nullptr;
	}

	template <class T>
	void UniquePtr<T>::reset() noexcept {
		delete m_underlyingPtr;
		m_underlyingPtr = nullptr;
	}

	template <class T>
	void swap(UniquePtr<T>& ptr1, UniquePtr<T>& ptr2) noexcept {
		std::swap(ptr1.m_underlyingPtr, ptr2.m_underlyingPtr);
	}

	template <class T, class... Args>
	UniquePtr<T> makeUnique(Args&&... args) {
		return { new T(std::forward<Args>(args)...) };
	}
}