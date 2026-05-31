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
    m_time("")
{}

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
    const std::string& time)
    : m_notificationId(id),
    m_receiver(receiver),
    m_message(message),
    m_status(Enums::NotificationStatus::SENT),
    m_time(time)
{}

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
const std::string& Notification::getTime() const
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
void Notification::setTime(const std::string& time)
{
    m_time = time;
}

/*
 * Function: serialize
 * Description: Converts Notification object into CSV format string
 * Returns:
 *    CSV string representing the user
 */
std::string Notification::serialize()
{
    std::string result = m_notificationId + config::delimeter::comma;
    if (m_receiver)
    {
        result += m_receiver->getUserId() + config::delimeter::comma;
    }
    result += m_message + config::delimeter::comma +
        Enums::getNotificationStatusString(m_status) + config::delimeter::comma +
        m_time;
    return result;
}