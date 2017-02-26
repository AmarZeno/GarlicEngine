#pragma once
#include "StrongPointer.h"
class GameObject;
template <class T>
class WeakPointer
{
public:
	explicit WeakPointer(T * i_ptr = NULL);

	// Copy Constructor
	WeakPointer(const WeakPointer<T> & i_other) :
		m_pObject(i_other.m_pObject),
		m_pCounters(i_other.m_pCounters)
	{
		// Increment the weak reference count
		(m_pCounters->weakCount)++;
	}

	WeakPointer(const StrongPointer<T> & i_other) :
		m_pObject(i_other.GetObject()),
		m_pCounters(i_other.GetCounters())
	{
		// Increment the weak reference count
		(m_pCounters->weakCount)++;
	};

	WeakPointer<T> & operator=(const WeakPointer<T> & i_other)
	{
		// Prevent for same object (self)
		if (this == &i_other) {
			return *this;
		}

		// If input is null, release the reference to gameObject
		if (i_other == nullptr) {
			DecrementWeakReference();
		}

		return *this;
	}

	WeakPointer<T> & operator=(const StrongPointer<T> & i_other)
	{
		// If input is null, release the reference to gameObject
		if (i_other == nullptr) {
			return *this;
		}
		else {
			m_pCounters = i_other.GetCounters();
			m_pObject = i_other.GetObject();
			(m_pCounters->weakCount)++;
		}

		return *this;
	}

	WeakPointer<T> & operator=(const nullptr_t) {
		DecrementWeakReference();
		return *this;
	}

	// Member access operator
	T * operator->() {
		return m_pObject;
	}

	// Indirection operator
	T & operator*() {
		assert(m_pObject != nullptr);
		return *m_pObject;
	}

	inline bool operator==(const WeakPointer<T> &i_pRhs) const;
	inline bool operator==(const nullptr_t) const;

	~WeakPointer();
	
	// Provide checks to see if underlying object is available
	inline bool IsValid() const;
	inline StrongPointer<T> Acquire() const;
	inline void DecrementWeakReference();

private:
	Counters * m_pCounters;
	T * m_pObject;
};


#include "WeakPointer-inl.h"