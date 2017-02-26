#pragma once
struct Counters {
	long refCount;
	long weakCount;
};

template<class T>
class StrongPointer
{
public:
	// Default Constructor
	//StrongPointer() :
	//	m_pObject(new GameObject(Vector2D(0, 0))),
	//	m_pCounters(new Counters)
	//{
	//	m_pCounters->refCount = 1;
	//	m_pCounters->weakCount = 0;
	//}

	// Standard Constructor
	explicit StrongPointer(T * i_pObject = nullptr) :
		m_pObject(i_pObject),
		m_pCounters(new Counters)
	{
		m_pCounters->refCount = 1;
		m_pCounters->weakCount = 0;
	}

	// Copy constructor
	StrongPointer(const StrongPointer<T> & i_other) :
		m_pObject(i_other.m_pObject),
		m_pCounters(i_other.m_pCounters) {
		// Increment the master reference count
		(m_pCounters->refCount)++;
	}

	// Assignment Operators
	StrongPointer<T> & operator=(const StrongPointer<T> & i_other)
	{
		// Prevent for same object (self)
		if (this == &i_other) {
			return *this;
		}

		ReleaseCurrentReference();
		// If input is null, just release the reference to gameObject
		if (i_other == nullptr) {
			// Do nothing
		}
		else {
			AcquireNewReference(i_other);
		}
		return *this;
	}

	StrongPointer<T> & operator=(const nullptr_t) {
		ReleaseCurrentReference();
		return *this;
	}

	StrongPointer(T * i_ptr, Counters * m_pCounters) :
		m_pObject(i_ptr),
		m_pCounters(m_pCounters)
	{
		// Do not increment the reference count
		(m_pCounters->refCount)++;
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

	inline bool operator==(const StrongPointer<T> &i_pRhs) const;
	inline bool operator==(const nullptr_t) const;

	~StrongPointer();

	inline T * GetObject() const;
	inline Counters * GetCounters() const;

	void ResetStrongCount() {
		m_pCounters->refCount = 0;
	}

private:
	T * m_pObject;
	Counters * m_pCounters;
	inline void ReleaseCurrentReference();
	inline void AcquireNewReference(const StrongPointer<T> & i_other);
};


#include "StrongPointer-inl.h"