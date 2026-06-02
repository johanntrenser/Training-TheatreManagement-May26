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
	: m_dataStore(DataStore::getInstance())
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
	const std::map<std::string, Notification*>& notifications = m_dataStore.getNotifications();
	int idNumber = static_cast<int>(notifications.size()) + 1;
	std::ostringstream buffer;
	buffer << "NF"
		<< std::setw(3)
		<< std::setfill('0')
		<< idNumber;
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
	m_dataStore.getNotifications()[notification->getNotificationId()] = notification;
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
	for (std::map<std::string, Notification*>::const_iterator iterator = notifications.begin(); iterator != notifications.end(); ++iterator)
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

/*
 * Function: NotificationManagementService::saveNotificationData
 * Description: Saves all notification data from the DataStore into a CSV file.
 *              Includes notification details such as Notification ID, Receiver ID,
 *              message content, status, and timestamp.
 *              Overwrites existing file content.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened)
 */
void NotificationManagementService::saveNotificationData()
{
	std::vector<std::string> lines;
	lines.push_back(config::Header::NOTIFICATION_HEADER);
	const std::map<std::string, Notification*>& notifications = m_dataStore.getNotifications();
	for (std::map<std::string, Notification*>::const_iterator iterator = notifications.begin(); iterator != notifications.end(); ++iterator)
	{
		lines.push_back((iterator->second)->serialize());
	}
	FileManagement::writeLines(std::string(config::File::NOTIFICATION_FILEPATH), lines);
}

/*
 * Function: NotificationManagementService::loadtNotificationData
 * Description: Loads all notification data from a CSV file into memory.
 *              Reads each line from the file using FileManagement::readlines(PATH),
 *              deserializes it into a Notification object via Notification::deserialize,
 *              and restores the association with its receiver User if the receiver ID
 *              is present and found in the DataStore. Also sets the Notification status
 *              using Enums::getNotificationStatus before adding the reconstructed
 *              Notification to the DataStore.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened or read)
 */
void NotificationManagementService::loadNotificationData()
{
	std::string notificationId, receiverId, message, status, time;
	std::vector<std::string> lines = FileManagement::readlines(PATH);
	for (int index = 1; index < lines.size(); ++index)
	{
		Notification* notification = Notification::deserialize(lines[index]);
		std::stringstream lineStream(lines[index]);
		getline(lineStream, notificationId, ',');
		getline(lineStream, receiverId, ',');
		getline(lineStream, message, ',');
		getline(lineStream, status, ',');
		getline(lineStream, time, ',');
		if (!receiverId.empty())
		{
			User* receiver = m_dataStore.getUserById(receiverId);
			notification->setReceiver(receiver);
		}
		Enums::NotificationStatus notificationStatus = Enums::getNotificationStatus(status);
		if (notificationStatus == Enums::NotificationStatus::FAILED || notificationStatus == Enums::NotificationStatus::UNREAD)
		{
			notificationStatus = Enums::NotificationStatus::UNREAD;
		}
		notification->setStatus(notificationStatus);
		m_dataStore.addNotification(notification);
	}
}