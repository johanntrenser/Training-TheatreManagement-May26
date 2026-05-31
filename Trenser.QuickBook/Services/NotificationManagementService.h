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
#include <fstream>
#include <map>
#include <sstream>
#include "FileManagement.h"
#include "DataStore.h"
#include "ApplicationConfig.h"
class NotificationManagementService
{
private:
	DataStore& m_dataStore;
	const std::string& PATH = config::File::NOTIFICATION_FILEPATH;
public:
	NotificationManagementService();
	void saveNotificationData();
	void loadtNotificationData();
};