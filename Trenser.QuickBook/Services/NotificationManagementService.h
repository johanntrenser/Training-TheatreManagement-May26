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
#include "DataStore.h"
#include "ApplicationConfig.h"
class NotificationManagementService
{
private:
	DataStore& m_dataStore;
	const std::string& PATH = config::File::MOVIE_FILEPATH;
public:
	NotificationManagementService();
	void saveNotificationData();
};