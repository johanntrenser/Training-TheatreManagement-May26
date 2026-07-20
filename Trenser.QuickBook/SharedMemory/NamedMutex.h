/*
 * File: NamedMutex.h
 * Description: Declares the NamedMutex class, which provides a wrapper
 *              around Windows named mutex objects for synchronizing access
 *              to shared resources across multiple threads and processes.
 *              The class supports mutex creation, locking, unlocking, and
 *              automatic handle cleanup.
 * Author: Trenser
 * Created: 12 June 2026
 */
#pragma once
#include <Windows.h>
#include <string>

class NamedMutex
{
private:
	HANDLE m_handle;
public:
	NamedMutex() = delete;
	NamedMutex(const std::string& name) :
		m_handle(NULL)
	{
		m_handle = CreateMutexA(NULL, FALSE, name.c_str());
	}
	NamedMutex(const NamedMutex&) = delete;
	NamedMutex& operator=(const NamedMutex&) = delete;
	void lock();
	void unlock();
	~NamedMutex()
	{
		if (m_handle != NULL)
		{
			CloseHandle(m_handle);
		}
	}
};

