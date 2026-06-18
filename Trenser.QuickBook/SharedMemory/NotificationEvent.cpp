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
 *              for inter-process notification communication for a specific user.
 *              Creates a file mapping for the NotifyMessage structure using a
 *              user-specific name, maps it into the process address space, and
 *              sets up a named event for signaling notifications. Also initializes
 *              a named mutex to ensure thread-safe access to the shared buffer.
 *              If any step fails, resources are cleaned up to prevent leaks.
 * Parameters:
 *    userId - The unique identifier of the user for whom the notification
 *             resources are being initialized.
 * Returns:
 *    None
 */
void NotificationEvent::init(const std::string& userId)
{
	std::string NotificationSharedMemory = config::FileMappings::NOTIFICATION_EVENT_MAPPING_NAME + userId;
	m_hMapFile = CreateFileMappingA(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(NotifyMessage),
		NotificationSharedMemory.c_str()
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
	std::string eventName = config::MutexMappings::NOTIFICATION_EVENT_MUTEX_NAME + userId;
	m_hEvent = CreateEventA(
		NULL,
		FALSE,
		FALSE,
		eventName.c_str()
	);
	if (m_hEvent == nullptr)
	{
		UnmapViewOfFile(m_pointerToBuffer);
		CloseHandle(m_hMapFile);
		m_pointerToBuffer = nullptr;
		m_hMapFile = nullptr;
	}
	m_mutex = new NamedMutex(config::MutexMappings::NOTIFICATION_MUTEX_NAME);
}

/*
 * Function: NotificationEvent::notify
 * Description: Sends a notification message to a user-specific shared memory buffer
 *              and signals the corresponding event. Validates that the event handle
 *              and mutex are initialized before proceeding. Acquires a scoped lock
 *              on the named mutex to ensure thread-safe access, creates or opens a
 *              shared memory segment identified by the targetId, writes the target
 *              type, target ID, and message into the NotifyMessage structure, and
 *              signals the user-specific event. Cleans up handles after signaling.
 * Parameters:
 *    targetType      - The type of target (e.g., "ALL", "USER_TYPE").
 *    targetId        - The specific user ID for targeted messages.
 *    incomingMessage - The notification message content.
 * Returns:
 *    None
 */
void NotificationEvent::notify(const std::string& targetType, const std::string& targetId, const std::string& incomingMessage)
{
	if (!m_hEvent || !m_mutex)
	{
		return;
	}
	ScopedLock lock(*m_mutex);
	std::string NotificationSharedMemory = config::FileMappings::NOTIFICATION_EVENT_MAPPING_NAME + targetId;
	m_hMapFile = CreateFileMappingA(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(NotifyMessage),
		NotificationSharedMemory.c_str()
	);
	if (m_hMapFile == nullptr)
	{
		return;
	}
	char* pointerToBuffer = (char*)MapViewOfFile(
		m_hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(NotifyMessage)
	);
	if (pointerToBuffer == nullptr)
	{
		CloseHandle(m_hMapFile);
		m_hMapFile = nullptr;
		return;
	}
	NotifyMessage* message = reinterpret_cast<NotifyMessage*>(pointerToBuffer);
	strncpy_s(message->targetType, targetType.c_str(), _TRUNCATE);
	strncpy_s(message->targetId, targetId.c_str(), _TRUNCATE);
	strncpy_s(message->message, incomingMessage.c_str(), _TRUNCATE);
	std::string eventName = config::MutexMappings::NOTIFICATION_EVENT_MUTEX_NAME + targetId;
	HANDLE targetEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, eventName.c_str());
	if (targetEvent != nullptr)
	{
		SetEvent(targetEvent);
		CloseHandle(targetEvent);
	}
}

/*
 * Function: NotificationEvent::notify
 * Description: Sends a notification message to multiple user-specific shared memory buffers
 *              and signals their corresponding events. Iterates through the list of target IDs,
 *              validates that the event handle and mutex are initialized, and acquires a scoped
 *              lock on the mutex for thread-safe access. For each target ID, creates or opens a
 *              shared memory segment identified by "NotificationSharedMemory_<targetId>", writes
 *              the target type, target ID, and message into the NotifyMessage structure, and signals
 *              the corresponding event "Event_<targetId>". Cleans up event handles after signaling.
 * Parameters:
 *    targetType      - The type of target (e.g., "ALL", "USER_TYPE").
 *    targetIds       - A vector of user IDs to which the notification should be sent.
 *    incomingMessage - The notification message content.
 * Returns:
 *    None
 */
void NotificationEvent::notify(const std::string& targetType,const std::vector<std::string>& targetIds, const std::string& incomingMessage)
{
	for (int index = 0; index < targetIds.size(); index++)
	{
		if (!m_hEvent || !m_mutex)
		{
			return;
		}
		ScopedLock lock(*m_mutex);
		std::string NotificationSharedMemory = config::FileMappings::NOTIFICATION_EVENT_MAPPING_NAME + targetIds[index];
		m_hMapFile = CreateFileMappingA(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(NotifyMessage),
			NotificationSharedMemory.c_str()
		);
		if (m_hMapFile == nullptr)
		{
			return;
		}
		char* pointerToBuffer = (char*)MapViewOfFile(
			m_hMapFile,
			FILE_MAP_ALL_ACCESS,
			0,
			0,
			sizeof(NotifyMessage)
		);
		if (pointerToBuffer == nullptr)
		{
			CloseHandle(m_hMapFile);
			m_hMapFile = nullptr;
			return;
		}
		NotifyMessage* message = reinterpret_cast<NotifyMessage*>(pointerToBuffer);
		strncpy_s(message->targetType, targetType.c_str(), _TRUNCATE);
		strncpy_s(message->targetId, targetIds[index].c_str(), _TRUNCATE);
		strncpy_s(message->message, incomingMessage.c_str(), _TRUNCATE);
		std::string eventName = config::MutexMappings::NOTIFICATION_EVENT_MUTEX_NAME + targetIds[index];
		HANDLE targetEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, eventName.c_str());
		if (targetEvent != nullptr)
		{
			SetEvent(targetEvent);
			CloseHandle(targetEvent);
		}
	}
}

/*
 * Function: NotificationEvent::startListener
 * Description: Starts a background listener thread that continuously waits for
 *              notification events from shared memory for a specific user. When
 *              an event is signaled, the listener acquires a scoped lock on the
 *              shared buffer, retrieves the message, and checks if it is intended
 *              for the current user (based on target type and target ID). If the
 *              message is relevant, it is displayed in the console using the
 *              displayNotification method, which prefixes the message with the
 *              user’s name and auto-clears it after a configured duration.
 * Parameters:
 *    currentUserType - The type of the current user (e.g., "ADMIN", "THEATRE_OWNER","CUSTOMER").
 *    currentUserId   - The unique identifier of the current user.
 *    userName        - The display name of the user, used to prefix notifications.
 * Returns:
 *    None
 */
void NotificationEvent::startListener(const std::string& currentUserType, const std::string& currentUserId, const std::string& userName)
{
	std::thread([currentUserType, currentUserId, userName]()
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
				bool isTheMessageForMe = false;
				if (targetType == config::UserType::ALL_USER)
				{
					isTheMessageForMe = true;
				}
				else if (targetType == currentUserType && !targetId.empty())
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
				displayNotification(message, userName);
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
 * Function: NotificationEvent::displayNotification
 * Description: Displays a notification message at the bottom of the Windows console
 *              for a specific user. Converts the message and username into a wide
 *              string, writes it to the console output buffer, and keeps it visible
 *              for a configured duration. After the delay, a detached thread clears
 *              the message by overwriting it with blank spaces, ensuring the console
 *              remains uncluttered.
 * Parameters:
 *    message  - The notification message content to display.
 *    userName - The name of the user associated with the notification.
 * Returns:
 *    None
 */
void NotificationEvent::displayNotification(const std::string& message,const std::string& userName)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	COORD position;
	position.X = 0;
	position.Y = csbi.srWindow.Bottom;
	DWORD written;
	std::wstring wideMessage = toWide(userName + config::delimeter::colon + message);
	WriteConsoleOutputCharacterW(hConsole, wideMessage.c_str(), (DWORD)wideMessage.size(), position, &written);
	std::this_thread::sleep_for(std::chrono::seconds(config::Limit::MAX_NOTIFICATION_TIMER));
	std::thread([hConsole, position, wideMessage]()
		{
			std::this_thread::sleep_for(std::chrono::seconds(config::Limit::MAX_NOTIFICATION_TIMER));
			DWORD written;
			std::wstring blank(wideMessage.size(), L' ');
			WriteConsoleOutputCharacterW(hConsole, blank.c_str(), (DWORD)blank.size(), position, &written);
		}).detach();
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