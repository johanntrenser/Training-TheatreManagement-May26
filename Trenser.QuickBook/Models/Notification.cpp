/*
 * File: Notification.cpp
 * Description: Implements the Notification class, providing constructors,
 *              getters, and setters for notification attributes such as
 *              notification ID, receiver, message, status, and time.
 * Author: Trenser
 * Created: 20 May 2026
 */
#include "Notification.h"

 /*
  * Function: Notification::Notification
  * Description: Default constructor. Initializes notification attributes to
  *              empty strings or nullptr, with status set to SENT.
  * Returns: None
  */
Notification::Notification()
    : m_notificationId(""),
    m_receiver(nullptr),
    m_message(""),
    m_status(Enums::NotificationStatus::SENT),
    m_time(-1)
{
}

/*
 * Function: Notification::Notification
 * Description: Parameterized constructor. Initializes notification attributes
 *              with provided values. Status defaults to SENT.
 * Parameters:
 *    const std::string& id - Unique notification identifier
 *    User* receiver - Pointer to the user receiving the notification
 *    const std::string& message - Notification message content
 *    const std::string& time - Time of notification
 * Returns: None
 */
Notification::Notification(const std::string& id,
    User* receiver,
    const std::string& message,
    time_t time)
    : m_notificationId(id),
    m_receiver(receiver),
    m_message(message),
    m_status(Enums::NotificationStatus::SENT),
    m_time(time)
{
}

/*
 * Function: Notification::getNotificationId
 * Description: Retrieves the notification ID.
 * Returns:
 *    const std::string& - Notification ID
 */
const std::string& Notification::getNotificationId() const
{
    return m_notificationId;
}

/*
 * Function: Notification::getReceiver
 * Description: Retrieves the receiver of the notification.
 * Returns:
 *    User* - Pointer to the receiver
 */
User* Notification::getReceiver() const
{
    return m_receiver;
}

/*
 * Function: Notification::getMessage
 * Description: Retrieves the notification message.
 * Returns:
 *    const std::string& - Notification message
 */
const std::string& Notification::getMessage() const
{
    return m_message;
}

/*
 * Function: Notification::getStatus
 * Description: Retrieves the notification status.
 * Returns:
 *    Enums::NotificationStatus - Current notification status
 */
Enums::NotificationStatus Notification::getStatus() const
{
    return m_status;
}

/*
 * Function: Notification::getTime
 * Description: Retrieves the notification time.
 * Returns:
 *    const std::string& - Notification time
 */
const time_t Notification::getTime() const
{
    return m_time;
}

/*
 * Function: Notification::setNotificationId
 * Description: Updates the notification ID.
 * Parameters:
 *    const std::string& id - New notification ID
 * Returns: None
 */
void Notification::setNotificationId(const std::string& id)
{
    m_notificationId = id;
}

/*
 * Function: Notification::setReceiver
 * Description: Updates the receiver of the notification.
 * Parameters:
 *    User* receiver - New receiver pointer
 * Returns: None
 */
void Notification::setReceiver(User* receiver)
{
    m_receiver = receiver;
}

/*
 * Function: Notification::setMessage
 * Description: Updates the notification message.
 * Parameters:
 *    const std::string& message - New notification message
 * Returns: None
 */
void Notification::setMessage(const std::string& message)
{
    m_message = message;
}

/*
 * Function: Notification::setStatus
 * Description: Updates the notification status.
 * Parameters:
 *    Enums::NotificationStatus status - New notification status
 * Returns: None
 */
void Notification::setStatus(Enums::NotificationStatus status)
{
    m_status = status;
}

/*
 * Function: Notification::setTime
 * Description: Updates the notification time.
 * Parameters:
 *    const std::string& time - New notification time
 * Returns: None
 */
void Notification::setTime(const time_t time)
{
    m_time = time;
}

/*
 * Function: serialize
 * Description: Converts Notification object into CSV format string
 * Returns:
 *    CSV string representing the user
 */
SharedNotification Notification::serialize()
{
    SharedNotification sharedNotification;
    strncpy_s(sharedNotification.notificationId, m_notificationId.c_str(), sizeof(sharedNotification.notificationId));
    strncpy_s(sharedNotification.userId, (m_receiver ? m_receiver->getUserId().c_str() : ""), sizeof(sharedNotification.userId));
    strncpy_s(sharedNotification.message, m_message.c_str(), sizeof(sharedNotification.message));
    sharedNotification.status = static_cast<int>(m_status);
    strncpy_s(sharedNotification.time, util::serializeTime(m_time).c_str(), sizeof(sharedNotification.time));
    return sharedNotification;
}

/*
 * Function: Notification::deserialize
 * Description: Converts a single CSV-formatted line into a Notification object.
 *              Extracts fields such as notificationId, receiverId, message, status,
 *              and time. The time string is parsed into its components (year, month,
 *              day, hour, minute) and converted into a time_t using util::createTime.
 *              The receiver User pointer is initialized to nullptr and can be set later
 *              when restoring relationships.
 * Parameters:
 *    lines - reference to a CSV-formatted string containing notification data
 * Returns:
 *    Pointer to a newly constructed Notification object
 */
Notification* Notification::deserialize(const SharedNotification* sharedNotification)
{
    if (sharedNotification == nullptr)
    {
        return nullptr;
    }
    Enums::NotificationStatus status = static_cast<Enums::NotificationStatus>(sharedNotification->status);
    time_t time = util::deserializeTime(sharedNotification->time);
    Notification* notification = Factory::getObject<Notification>(
        sharedNotification->notificationId,
        nullptr,
        sharedNotification->message,
        time
    );
    notification->setStatus(status);
    return notification;
}