/*
 * File: SessionManager.cpp
 * Description: Implements the SessionManager class, which manages user login
 *              sessions through a shared memory mapping. Provides functionality
 *              to initialize shared session storage, determine whether users
 *              are currently logged in, add and remove sessions, and clean up
 *              mapped memory resources.
 * Author: Trenser
 * Created: 15 June 2026
 */
#include "SessionManager.h"

/*
* Function: SessionManager::SessionManager
* Description: Initializes the SessionManager object by setting all mapping
*              handles and pointers to their default null states.
* Parameters:
*    None
* Returns:
*    None
*/
SessionManager::SessionManager():
	m_mappingHandle(NULL),
	m_filePointer(nullptr),
	m_sessions(nullptr)
{
}

/*
* Function: SessionManager::open
* Description: Creates or opens a named shared memory mapping used to store
*              active user sessions and maps it into the process address space.
*              If the mapping is created for the first time, the session count
*              is initialized to zero.
* Parameters:
*    None
* Returns:
*    true if the shared session mapping was opened successfully,
*    false otherwise.
*/
bool SessionManager::open()
{
	m_mappingHandle = CreateFileMappingA(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(SharedSession),
		config::FileMappings::SESSION_MAPPING_NAME
	);
	if (m_mappingHandle == NULL)
	{
		return false;
	}
	m_filePointer = MapViewOfFile(
		m_mappingHandle,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(SharedSession)
	);
	if (m_filePointer == nullptr)
	{
		CloseHandle(m_mappingHandle);
		m_mappingHandle = NULL;
		return false;
	}
	m_sessions = reinterpret_cast<SharedSession*>(m_filePointer);
	if (GetLastError() != ERROR_ALREADY_EXISTS)
	{
		m_sessions->sessionCount = 0;
	}
	return true;
}

/*
* Function: SessionManager::isLoggedIn
* Description: Checks whether the specified user currently has an active
*              session in the shared session store.
* Parameters:
*    userId - Unique identifier of the user to be checked.
* Returns:
*    true if the user is currently logged in,
*    false otherwise.
*/
bool SessionManager::isLoggedIn(const std::string& userId)
{
	if (m_sessions == nullptr)
	{
		return false;
	}
	for (int index = 0; index < m_sessions->sessionCount; ++index)
	{
		SessionEntry& session = m_sessions->sessions[index];
		if (strcmp(session.userId, userId.c_str()) == 0)
		{
			HANDLE process = OpenProcess(
					PROCESS_QUERY_LIMITED_INFORMATION,
					FALSE,
					session.processId);
			if (process == NULL)
			{
				removeSession(userId);
				return false;
			}
			CloseHandle(process);
			return true;
		}
	}
	return false;
}

/*
* Function: SessionManager::addSession
* Description: Adds a new active session for the specified user by storing
*              the user ID and current process ID in the shared session store.
*              If the user already has an active session, no new session is added.
* Parameters:
*    userId - Unique identifier of the user whose session is to be added.
* Returns:
*    true if the session was added successfully,
*    false otherwise.
*/
bool SessionManager::addSession(const std::string& userId)
{
	if (isLoggedIn(userId))
	{
		return false;
	}
	if (m_sessions->sessionCount >= config::Limit::SESSION_MAX_COUNT)
	{
		return false;
	}
	int index = m_sessions->sessionCount;
	strncpy_s(m_sessions->sessions[index].userId, userId.c_str(), sizeof(m_sessions->sessions[index].userId));
	m_sessions->sessions[index].processId = GetCurrentProcessId();
	m_sessions->sessionCount++;
	return true;
}

/*
* Function: SessionManager::removeSession
* Description: Removes the active session associated with the specified user.
*              The remaining sessions are shifted to maintain a contiguous
*              session array.
* Parameters:
*    userId - Unique identifier of the user whose session is to be removed.
* Returns:
*    true if the session was removed successfully,
*    false otherwise.
*/
bool SessionManager::removeSession(const std::string& userId)
{
	int removeIndex = -1;
	for (int index = 0; index < m_sessions->sessionCount; ++index)
	{
		if (strcmp(m_sessions->sessions[index].userId, userId.c_str()) == 0)
		{
			removeIndex = index;
			break;
		}
	}
	if (removeIndex == -1)
	{
		return false;
	}
	for (int index = removeIndex; index < m_sessions->sessionCount - 1; ++index)
	{
		m_sessions->sessions[index] = m_sessions->sessions[index + 1];
	}
	m_sessions->sessionCount--;
	memset(&m_sessions->sessions[m_sessions->sessionCount], 0, sizeof(SessionEntry));
	return true;
}

/*
* Function: SessionManager::close
* Description: Unmaps the shared session memory region and releases the
*              associated mapping handle. Internal pointers are reset to
*              their default null states.
* Parameters:
*    None
* Returns:
*    None
*/
void SessionManager::close()
{
	if (m_filePointer)
	{
		UnmapViewOfFile(m_filePointer);
		m_filePointer = nullptr;
		m_sessions = nullptr;
	}
	if (m_mappingHandle)
	{
		CloseHandle(m_mappingHandle);
		m_mappingHandle = NULL;
	}
}

/*
* Function: SessionManager::~SessionManager
* Description: Destructor for the SessionManager class. Releases all shared
*              memory resources associated with the session manager by
*              invoking close().
* Parameters:
*    None
* Returns:
*    None
*/
SessionManager::~SessionManager()
{
	close();
}
