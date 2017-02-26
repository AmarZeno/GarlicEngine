#pragma once
#include "StrongPointer.h"
#include "GameObject.h"

template<class T>
inline bool StrongPointer<T>::operator==(const StrongPointer<T> & i_pRhs) const
{
	if (this == &i_pRhs)
		return true;
	return false;
}

template<class T>
inline bool StrongPointer<T>::operator==(const nullptr_t) const
{
	if (this == nullptr) {
		return true;
	}
	return false;
}

template<class T>
inline StrongPointer<T>::~StrongPointer()
{
	ReleaseCurrentReference();
}

template<class T>
inline T * StrongPointer<T>::GetObject() const
{
	return m_pObject;
}

template<class T>
inline Counters * StrongPointer<T>::GetCounters() const
{
	return m_pCounters;
}

template<class T>
inline void StrongPointer<T>::ReleaseCurrentReference()
{
	if (--(m_pCounters->refCount) == 0) {
		delete m_pCounters;
		delete m_pObject;
	}
	m_pCounters = nullptr;
	m_pObject = nullptr;
}

template<class T>
inline void StrongPointer<T>::AcquireNewReference(const StrongPointer<T> & i_other)
{
	m_pObject = i_other.m_pObject;
	m_pCounters->refCount = ++i_other.m_pCounters->refCount;
	m_pCounters->weakCount = i_other.m_pCounters->weakCount;
}
