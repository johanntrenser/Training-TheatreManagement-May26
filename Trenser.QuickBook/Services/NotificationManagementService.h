/*
 * File: NotificationManagementService.h
 * Description: Declares the NotificationManagementService class, which provides
 *              functionality for managing notifications within the system.
 *              This service will handle operations such as creating, sending,
 *              and clearing notifications for users.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include "DataStore.h"

class NotificationManagementService
{
private:
	DataStore& m_dataStore;
public:
	NotificationManagementService();
	std::string generateNotificationId();
	Enums::ProcessStatus sendNotification(User* receiver, const std::string& message);
	Enums::ProcessStatus sendNotificationToAllUsers(const std::string& message);
};