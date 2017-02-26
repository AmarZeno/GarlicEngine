#pragma once
#include "WeakPointer.h"

template<class T>
inline WeakPointer<T>::WeakPointer(T * i_ptr)
{
}

template<class T>
inline bool WeakPointer<T>::operator==(const WeakPointer<T> & i_pRhs) const
{
	if (this == &i_pRhs)
		return true;
	return false;
}

template<class T>
inline bool WeakPointer<T>::operator==(const nullptr_t) const
{
	if (this == nullptr) {
		return true;
	}
	return false;
}

template<class T>
inline WeakPointer<T>::~WeakPointer()
{
	//DecrementWeakReference();
}

template<class T>
inline bool WeakPointer<T>::IsValid() const
{
	return Acquire().GetObject() != nullptr;
}

template<class T>
inline StrongPointer<T> WeakPointer<T>::Acquire() const
{
	if (m_pCounters->refCount > 0) {
		StrongPointer<GameObject> strPtr(m_pObject, m_pCounters);
		return strPtr;
	}
	else {
		return StrongPointer<GameObject>(nullptr);
	}
}

template<class T>
inline void WeakPointer<T>::DecrementWeakReference()
{
	(m_pCounters->weakCount)--;
}
