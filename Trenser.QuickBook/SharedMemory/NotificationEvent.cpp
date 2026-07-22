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
        return;
    }
    m_mutex = new NamedMutex(std::string(config::MutexMappings::NOTIFICATION_MUTEX_EVENT_NAME) + userId);
}

/*
 * Function: NotificationEvent::notify
 * Description: Sends a notification message to a specific user’s shared memory buffer
 *              and signals their corresponding event. Validates that the event handle
 *              and global mutex are initialized before proceeding. Creates a user-specific
 *              NamedMutex to ensure thread-safe access, then opens or creates a shared
 *              memory segment identified by "NOTIFICATION_EVENT_MAPPING_NAME + targetId".
 *              Writes the target type, target ID, and message into the NotifyMessage
 *              structure, unmaps and closes the memory view, and signals the corresponding
 *              event "NOTIFICATION_EVENT_MUTEX_NAME + targetId". Cleans up handles after
 *              signaling to prevent resource leaks.
 * Parameters:
 *    targetType      - The type of target (e.g., "ALL", "USER_TYPE").
 *    targetId        - The specific user ID for the targeted message.
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
    HANDLE hMapFile = CreateFileMappingA(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        sizeof(NotifyMessage),
        NotificationSharedMemory.c_str()
    );
    if (hMapFile == nullptr)
    {
        return;
    }
    char* pointerToBuffer = (char*)MapViewOfFile(
        hMapFile,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        sizeof(NotifyMessage)
    );
    if (pointerToBuffer == nullptr)
    {
        CloseHandle(hMapFile);
        return;
    }
    NotifyMessage* message = reinterpret_cast<NotifyMessage*>(pointerToBuffer);
    strncpy_s(message->targetType, targetType.c_str(), _TRUNCATE);
    strncpy_s(message->targetId, targetId.c_str(), _TRUNCATE);
    strncpy_s(message->message, incomingMessage.c_str(), _TRUNCATE);
    std::string eventName = config::MutexMappings::NOTIFICATION_EVENT_MUTEX_NAME + targetId;
    UnmapViewOfFile(pointerToBuffer);
    CloseHandle(hMapFile);
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
 *              and signals their corresponding events. Validates that the event handle and
 *              mutex are initialized before proceeding. Acquires a scoped lock on the mutex
 *              to ensure thread-safe access, then iterates through the list of target IDs.
 *              For each target ID, creates or opens a shared memory segment identified by
 *              "NOTIFICATION_EVENT_MAPPING_NAME + targetId", writes the target type, target ID,
 *              and message into the NotifyMessage structure, unmaps and closes the memory view,
 *              and signals the corresponding event "NOTIFICATION_EVENT_MUTEX_NAME + targetId".
 *              Cleans up handles after each iteration to prevent resource leaks.
 * Parameters:
 *    targetType      - The type of target (e.g., "ALL", "USER_TYPE").
 *    targetIds       - A vector of user IDs to which the notification should be sent.
 *    incomingMessage - The notification message content.
 * Returns:
 *    None
 */
void NotificationEvent::notify(const std::string& targetType, const std::vector<std::string>& targetIds, const std::string& incomingMessage)
{
    if (!m_hEvent || !m_mutex)
    {
        return;
    }
    ScopedLock lock(*m_mutex);
    for (int index = 0; index < targetIds.size(); index++)
    {
        std::string NotificationSharedMemory = config::FileMappings::NOTIFICATION_EVENT_MAPPING_NAME + targetIds[index];
        HANDLE hMapFile = CreateFileMappingA(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            sizeof(NotifyMessage),
            NotificationSharedMemory.c_str()
        );
        if (hMapFile == nullptr)
        {
            continue;
        }
        char* pointerToBuffer = (char*)MapViewOfFile(
            hMapFile,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            sizeof(NotifyMessage)
        );
        if (pointerToBuffer == nullptr)
        {
            CloseHandle(hMapFile);
            continue;
        }
        NotifyMessage* message = reinterpret_cast<NotifyMessage*>(pointerToBuffer);
        strncpy_s(message->targetType, targetType.c_str(), _TRUNCATE);
        strncpy_s(message->targetId, targetIds[index].c_str(), _TRUNCATE);
        strncpy_s(message->message, incomingMessage.c_str(), _TRUNCATE);
        std::string eventName = config::MutexMappings::NOTIFICATION_EVENT_MUTEX_NAME + targetIds[index];
        UnmapViewOfFile(pointerToBuffer);
        CloseHandle(hMapFile);
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
 * Description: Starts a background listener thread that waits for notification events
 *              from shared memory. When a message is targeted to the current user,
 *              it invokes the provided callback with the formatted message.
 * Parameters:
 *    currentUserType (const std::string&) - Type of the current user (e.g., Admin, Customer, TheatreOwner)
 *    currentUserId (const std::string&) - Unique identifier of the current user
 *    userName (const std::string&) - Display name of the current user
 *    onMessage (std::function<void(const std::string&)>) - Callback function invoked when a message is received
 * Returns:
 *    void
 */
void NotificationEvent::startListener(const std::string& currentUserType, const std::string& currentUserId, const std::string& userName, std::function<void(const std::string&)> onMessage)
{
    std::thread([currentUserType, currentUserId, userName, onMessage]()
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
                onMessage(userName + config::delimeter::colon + message);
            }
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