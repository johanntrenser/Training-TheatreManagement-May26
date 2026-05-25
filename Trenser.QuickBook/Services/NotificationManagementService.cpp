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