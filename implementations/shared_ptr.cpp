#include "shared_ptr.h"

namespace implementations {
	template <class T>
	SharedPtr<T>::SharedPtr(T* ptr)
		: m_underlyingPtr{ ptr }
		, m_refCount{ new int(ptr == nullptr ? 0 : 1) } {
	}

	template <class T>
	SharedPtr<T>::~SharedPtr() {
		if (m_underlyingPtr == nullptr) {
			return;
		}
		(*m_refCount)--;
		if (*m_refCount <= 0) {
			delete m_refCount;
			delete m_underlyingPtr;
		}
	}

	template <class T>
	T* SharedPtr<T>::get() noexcept {
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
}
