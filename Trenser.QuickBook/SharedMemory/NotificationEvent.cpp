/*
 * File: NotificationEvent.cpp
 * Description: Implements the NotificationEvent class, which provides inter-process
 *              communication for notifications using shared memory, named events,
 *              and mutex synchronization. This implementation includes initialization
 *              of IPC resources, sending notifications, starting a listener thread
 *              for incoming messages, converting UTF-8 strings to wide strings for
 *              console output, and proper cleanup of allocated resources.
 * Author: Trenser
 * Created: 17 June 2026
 */
#include "NotificationEvent.h"

HANDLE NotificationEvent::m_hMapFile = nullptr;
HANDLE NotificationEvent::m_hEvent = nullptr;
char* NotificationEvent::m_pointerToBuffer = nullptr;
NamedMutex* NotificationEvent::m_mutex = nullptr;
const size_t NotificationEvent::m_SHARED_BUFFER_SIZE = config::Limit::MAX_SHARED_BUFFER_SIZE;

/*
 * Function: NotificationEvent::init
 * Description: Initializes the shared memory, event, and mutex resources required
 *              for inter-process notification communication. Creates a file mapping
 *              for the NotifyMessage structure, maps it into the process address space,
 *              and sets up a named event for signaling. Also initializes a named mutex
 *              to ensure thread-safe access to the shared buffer. If any step fails,
 *              resources are cleaned up to prevent leaks.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void NotificationEvent::init()
{
	m_hMapFile = CreateFileMappingA(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(NotifyMessage),
		"NotificationSharedMemory" 
	);
	if (m_hMapFile == nullptr)
	{
		return;
	}
	m_pointerToBuffer = (char*)MapViewOfFile(
		m_hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(NotifyMessage)
	);
	if (m_pointerToBuffer == nullptr)
	{
		CloseHandle(m_hMapFile);
		m_hMapFile = nullptr;
		return;
	}
	m_hEvent = CreateEventA(
		NULL,
		TRUE,
		FALSE,
		"Event"
	);
	if (m_hEvent == nullptr)
	{
		UnmapViewOfFile(m_pointerToBuffer);
		CloseHandle(m_hMapFile);
		m_pointerToBuffer = nullptr;
		m_hMapFile = nullptr;
	}
	m_mutex = new NamedMutex("NotificationMutex");
}

/*
 * Function: NotificationEvent::notify
 * Description: Sends a notification message to the shared memory buffer and signals
 *              the event to notify listeners. Validates that shared resources
 *              (buffer, event, mutex) are initialized before proceeding.
 *              Acquires a scoped lock on the named mutex to ensure thread-safe
 *              access to the shared buffer, writes the target type, target ID,
 *              and message into the NotifyMessage structure, and sets the event
 *              to signal that a new message is available.
 * Parameters:
 *    targetType      - The type of target (e.g., "ALL", "USER_TYPE").
 *    targetId        - The specific user ID for targeted messages.
 *    incomingMessage - The notification message content.
 * Returns:
 *    None
 */
void NotificationEvent::notify(const std::string& targetType, const std::string& targetId, const std::string& incomingMessage)
{
	if (!m_pointerToBuffer || !m_hEvent || !m_mutex)
	{
		return;
	}
	ScopedLock lock(*m_mutex);
	NotifyMessage* message = reinterpret_cast<NotifyMessage*>(m_pointerToBuffer);
	strncpy_s(message->targetType, targetType.c_str(), _TRUNCATE);
	strncpy_s(message->targetId, targetId.c_str(), _TRUNCATE);
	strncpy_s(message->message, incomingMessage.c_str(), _TRUNCATE);
	if (!SetEvent(m_hEvent))
	{
		return;
	}
}

/*
 * Function: NotificationEvent::startListener
 * Description: Starts a background listener thread that continuously waits for
 *              notification events from shared memory. When an event is signaled,
 *              the listener acquires a scoped lock on the shared buffer, retrieves
 *              the message, and checks if it is intended for the current user
 *              (based on target type and target ID). If the message is relevant,
 *              it is displayed at the bottom of the console window. The message
 *              is automatically cleared after 10 seconds by a detached thread.
 * Parameters:
 *    currentUserType - The type of the current user (e.g., "ADMIN", "CLIENT").
 *    currentUserId   - The unique identifier of the current user.
 * Returns:
 *    None
 */
void NotificationEvent::startListener(const std::string& currentUserType, const std::string& currentUserId)
{
	std::thread([currentUserType, currentUserId]()
		{
			while (true)
			{
				WaitForSingleObject(m_hEvent, INFINITE);
				std::string targetType, targetId, message;
				{
					ScopedLock lock(*m_mutex);
					NotifyMessage* currentMessage = reinterpret_cast<NotifyMessage*>(m_pointerToBuffer);
					targetType = currentMessage->targetType;
					targetId = currentMessage->targetId;
					message = currentMessage->message;
				}
				ResetEvent(m_hEvent);

				bool isTheMessageForMe = false;
				if (targetType == "ALL")
				{
					isTheMessageForMe = true;
				}
				else if (targetType == currentUserType && targetId.empty())
				{
					isTheMessageForMe = true;
				}
				else if (targetId == currentUserId)
				{
					isTheMessageForMe = true;
				}
				if (!isTheMessageForMe)
				{
					continue;
				}

				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				CONSOLE_SCREEN_BUFFER_INFO csbi;
				GetConsoleScreenBufferInfo(hConsole, &csbi);

				COORD position;
				position.X = 0;
				position.Y = csbi.srWindow.Bottom;

				DWORD written;
				std::wstring wideMessage = toWide(message);
				WriteConsoleOutputCharacterW(hConsole, wideMessage.c_str(), (DWORD)wideMessage.size(), position, &written);

				std::thread([hConsole, position, wideMessage]()
					{
						std::this_thread::sleep_for(std::chrono::seconds(10));
						DWORD written;
						std::wstring blank(wideMessage.size(), L' ');
						WriteConsoleOutputCharacterW(hConsole, blank.c_str(), (DWORD)blank.size(), position, &written);
					}).detach();
				}
		}).detach();
}

/*
 * Function: NotificationEvent::toWide
 * Description: Converts a UTF-8 encoded std::string into a wide-character std::wstring
 *              using std::wstring_convert with the UTF-8 to UTF-16 codecvt facet.
 *              This is primarily used for displaying notification messages in the
 *              Windows console, which requires wide-character output functions.
 * Parameters:
 *    inputString - The UTF-8 encoded string to be converted.
 * Returns:
 *    A std::wstring containing the UTF-16 representation of the input string.
 */
std::wstring NotificationEvent::toWide(const std::string& inputString)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(inputString);
}

/*
 * Function: NotificationEvent::~NotificationEvent
 * Description: Cleans up resources allocated for inter-process notification communication.
 *              Unmaps the shared memory buffer, closes the file mapping and event handles,
 *              and deletes the named mutex to prevent resource leaks. Ensures that all
 *              static members are reset to nullptr after cleanup.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
NotificationEvent::~NotificationEvent()
{
	if (m_pointerToBuffer)
	{
		UnmapViewOfFile(m_pointerToBuffer);
		m_pointerToBuffer = nullptr;
	}
	if (m_hMapFile)
	{
		CloseHandle(m_hMapFile);
		m_hMapFile = nullptr;
	}
	if (m_hEvent)
	{
		CloseHandle(m_hEvent);
		m_hEvent = nullptr;
	}
	if (m_mutex)
	{
		delete m_mutex;
		m_mutex = nullptr;
	}
}