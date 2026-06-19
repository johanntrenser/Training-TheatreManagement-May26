/*
 * File: NotificationManagementService.cpp
 * Description: Declares the NotificationManagementService class, which provides
 *              functionality for managing notifications within the system.
 *              This service will handle operations such as creating, sending,
 *              and clearing notifications for users.
 * Author: Trenser
 * Created: 20 May 2026
 */
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <ctime>
#include "NotificationManagementService.h"
#include "Factory.h"

/*
* Function Name : NotificationManagementService
* Description   : Initializes the notification management service and connects it with the datastore.
* Parameters    : None
* Return Type   : Constructor
*/
NotificationManagementService::NotificationManagementService()
	: m_dataStore(DataStore::getInstance()),m_notificationMutex(config::MutexMappings::NOTIFICATION_MUTEX_NAME)
{
}

/*
* Function Name : generateNotificationId
* Description   : Generates a unique ID for a notification.
* Parameters    : None
* Return Type   : std::string
*/
std::string NotificationManagementService::generateNotificationId()
{
	ScopedLock lock(m_notificationMutex);
	const int notificationCount = m_dataStore.getNotificationsCount();
	int idNumber = notificationCount + 1;
	std::ostringstream buffer;
	buffer << "NF" << std::setw(3) << std::setfill('0') << idNumber;
	return buffer.str();
}

/*
* Function Name : sendNotification
* Description   : Sends a notification to a specific user.
* Parameters    :
*                  receiver - User receiving the notification
*                  message  - Notification message
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus NotificationManagementService::sendNotification(User* receiver, const std::string& message)
{
	if (!receiver)
	{
		return Enums::ProcessStatus::FAILED;
	}
	Notification* notification = Factory::getObject<Notification>(generateNotificationId(), receiver, message, time(nullptr));
	if (!notification)
	{
		return Enums::ProcessStatus::FAILED;
	}
	notification->setStatus(Enums::NotificationStatus::UNREAD);
	m_dataStore.addNotification(notification);
	return Enums::ProcessStatus::SUCCESS;
}

/*
* Function Name : sendNotificationToAllUsers
* Description   : Sends a notification to all users.
* Parameters    :
*                  message - Notification message
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus NotificationManagementService::sendNotificationToAllUsers(const std::string& message)
{
	const std::map<std::string, User*>& users = m_dataStore.getUsers();

	for (std::map<std::string, User*>::const_iterator iterator = users.begin(); iterator != users.end(); ++iterator)
	{
		sendNotification(iterator->second, message);
	}
	return Enums::ProcessStatus::SUCCESS;
}

/*
* Function Name : getUnreadNotifications
* Description   : Retrieves unread notifications for the authenticated user.
* Parameters    :
*                  batchSize             - Number of notifications to retrieve
*                  remainingUnreadCount  - Count of remaining unread notifications
* Return Type   : std::vector<std::string>
*/
std::vector<std::string> NotificationManagementService::getUnreadNotifications(int batchSize, int& remainingUnreadCount)
{
	std::vector<std::string> unreadNotifications;
	const User* currentUser = m_dataStore.getAuthenticatedUser();
	const std::map<std::string, Notification*>& notifications = m_dataStore.getNotifications();
	int displayedCount = 0;
	for (std::map<std::string, Notification*>::const_reverse_iterator iterator = notifications.rbegin(); iterator != notifications.rend(); ++iterator)
	{
		Notification* notification = iterator->second;
		if (notification->getReceiver() == currentUser && notification->getStatus() == Enums::NotificationStatus::UNREAD)
		{
			if (displayedCount < batchSize)
			{
				std::string notificationMessage = convertNotificationObjectToStringFormat(notification);
				unreadNotifications.push_back(notificationMessage);
				notification->setStatus(Enums::NotificationStatus::READ);
				++displayedCount;
			}
			else
			{
				++remainingUnreadCount;
			}
		}
	}
	return unreadNotifications;
}

/*
* Function Name : convertNotificationObjectToStringFormat
* Description   : Converts a notification object into a formatted string.
* Parameters    :
*                  notification - Notification object to format
* Return Type   : std::string
*/
std::string NotificationManagementService::convertNotificationObjectToStringFormat(const Notification* notification)
{
	time_t notificationTime = notification->getTime();
	char buffer[26];
	ctime_s(buffer, sizeof(buffer), &notificationTime);
	std::string timeString = buffer;
	if (!timeString.empty() && timeString[timeString.length() - 1] == '\n')
	{
		timeString.erase(timeString.length() - 1);
	}
	std::string notificationMessage = "[" + timeString + "] : " + notification->getMessage();
	return notificationMessage;
}