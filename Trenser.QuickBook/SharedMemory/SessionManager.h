/*
 * File: SessionManager.h
 * Description: Declares the SessionManager class, which manages user login
 *              sessions using a shared memory mapping. Provides functionality
 *              to create and access a shared session store, track active users,
 *              add and remove sessions, and release shared memory resources.
 * Author: Trenser
 * Created: 15 June 2026
 */
#pragma once
#include <Windows.h>
#include <string>
#include "SharedStructs.h"

class SessionManager
{
private:
	HANDLE m_mappingHandle;
	void* m_filePointer;
	SharedSession* m_sessions;
public:
	SessionManager();
	SessionManager(const SessionManager&) = delete;
	SessionManager& operator=(const SessionManager&) = delete;
	bool open();
	bool isLoggedIn(const std::string& userId);
	bool addSession(const std::string& userId);
	bool removeSession(const std::string& userId);
	void close();
	~SessionManager();
};

