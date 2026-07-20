/*
 * File: ScopedLock.h
 * Description: Declares the ScopedLock class, which provides automatic
 *              acquisition and release of a NamedMutex using the RAII
 *              (Resource Acquisition Is Initialization) pattern. The class
 *              ensures that a mutex is locked upon construction and
 *              automatically unlocked when the ScopedLock object goes out
 *              of scope, preventing resource leaks and deadlocks caused by
 *              missed unlock operations.
 * Author: Trenser
 * Created: 11 June 2026
 */
#pragma once
#include "NamedMutex.h"

class ScopedLock
{
private:
	NamedMutex& m_mutex;
public:
	ScopedLock(NamedMutex& mutex) :
		m_mutex(mutex)
	{
		m_mutex.lock();
	}
	ScopedLock(const ScopedLock&) = delete;
	ScopedLock& operator=(const ScopedLock&) = delete;
	~ScopedLock()
	{
		m_mutex.unlock();
	}
};

